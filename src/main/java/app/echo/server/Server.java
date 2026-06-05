package app.echo.server;

import app.echo.common.ChatDTO;
import app.echo.common.MessageDTO;
import app.echo.common.Packet;
import app.echo.common.PacketType;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.java_websocket.WebSocket;
import org.java_websocket.handshake.ClientHandshake;
import org.java_websocket.server.WebSocketServer;

import java.net.InetSocketAddress;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

public class Server extends WebSocketServer {
    private final Db db = new Db();
    private final ObjectMapper json = new ObjectMapper();

    // онлайн-пользователи: сокет -> логин
    private final Map<WebSocket, String> sessions = new ConcurrentHashMap<>();

    public Server(int port) { super(new InetSocketAddress(port)); }

    @Override public void onStart() { System.out.println("[SRV] запущен на порту " + getPort()); }

    @Override public void onOpen(WebSocket c, ClientHandshake h) {
        System.out.println("[SRV] connect " + c.getRemoteSocketAddress());
    }

    @Override public void onClose(WebSocket c, int code, String reason, boolean remote) {
        String who = sessions.remove(c);
        if (who != null) {
            System.out.println("[SRV] " + who + " offline");
            broadcastPresence();
        }
    }

    @Override public void onError(WebSocket c, Exception e) { e.printStackTrace(); }

    @Override public void onMessage(WebSocket c, String raw) {
        try {
            Packet p = json.readValue(raw, Packet.class);
            switch (p.type) {
                case REGISTER       -> onRegister(c, p);
                case LOGIN          -> onLogin(c, p);
                case LOGOUT         -> onLogout(c);
                case SEARCH         -> onSearch(c, p);
                case CREATE_PRIVATE -> onCreatePrivate(c, p);
                case CREATE_GROUP   -> onCreateGroup(c, p);
                case OPEN_CHAT      -> onOpenChat(c, p);
                case ADD_MEMBER     -> onAddMember(c, p);
                case LEAVE_CHAT     -> onLeaveChat(c, p);
                case MEMBERS        -> onMembers(c, p);
                case MESSAGE        -> onMessage(c, p);
                case DELETE_MESSAGE -> onDeleteMessage(c, p);
                case TYPING         -> onTyping(c, p);
                default -> {}
            }
        } catch (Exception e) {
            System.err.println("[SRV] bad packet: " + e.getMessage());
        }
    }

    // ─── auth ───────────────────────────────────────────────────────────────

    private void onRegister(WebSocket c, Packet p) {
        boolean ok = db.register(p.login, Security.hash(p.password));
        send(c, ok ? new Packet(PacketType.AUTH_OK, "Аккаунт создан! Теперь войдите.")
                   : new Packet(PacketType.AUTH_FAIL, "Логин уже занят"));
    }

    private void onLogin(WebSocket c, Packet p) {
        if (db.auth(p.login, Security.hash(p.password))) {
            sessions.put(c, p.login);
            send(c, new Packet(PacketType.AUTH_OK, "LOGGED_IN"));
            sendChatList(c, p.login);
            broadcastPresence();
        } else {
            send(c, new Packet(PacketType.AUTH_FAIL, "Неверный логин или пароль"));
        }
    }

    private void onLogout(WebSocket c) {
        String who = sessions.remove(c);
        if (who != null) broadcastPresence();
    }

    // ─── search / chats ────────────────────────────────────────────────────

    private void onSearch(WebSocket c, Packet p) {
        String me = sessions.get(c);
        if (me == null) return;
        String q = p.query == null ? "" : p.query.trim();
        List<ChatDTO> result = q.isEmpty()
                ? db.userChats(me, sessions.keySet().stream().map(sessions::get).collect(java.util.stream.Collectors.toSet()))
                : db.search(q, me, onlineSet());
        Packet resp = new Packet(PacketType.CHAT_LIST);
        resp.chats = result;
        resp.query = q;
        send(c, resp);
    }

    private void onCreatePrivate(WebSocket c, Packet p) {
        String me = sessions.get(c);
        if (me == null) return;
        if (!db.userExists(p.target)) { send(c, err("Пользователь не найден")); return; }
        int chatId = db.createPrivate(me, p.target);
        if (chatId < 0) { send(c, err("Не удалось создать чат")); return; }

        Packet opened = new Packet(PacketType.CHAT_OPENED);
        opened.chatId = chatId;
        opened.text = p.target;     // имя для отображения
        opened.count = 2;
        opened.query = "PRIVATE";   // признак типа чата
        send(c, opened);

        sendChatList(c, me);
        pushChatListTo(p.target);   // если партнёр онлайн
    }

    private void onCreateGroup(WebSocket c, Packet p) {
        String me = sessions.get(c);
        if (me == null) return;
        int chatId = db.createGroup(p.text, me);
        if (chatId < 0) { send(c, err("Не удалось создать группу")); return; }

        Packet opened = new Packet(PacketType.CHAT_OPENED);
        opened.chatId = chatId;
        opened.text = p.text;
        opened.count = 1;
        opened.query = "GROUP";     // признак типа чата
        send(c, opened);

        sendChatList(c, me);
    }

    private void onOpenChat(WebSocket c, Packet p) {
        String me = sessions.get(c);
        if (me == null) return;
        if (!db.members(p.chatId).contains(me)) { send(c, err("Нет доступа к чату")); return; }

        db.markRead(p.chatId, me);

        Packet hist = new Packet(PacketType.HISTORY);
        hist.chatId = p.chatId;
        hist.messages = db.history(p.chatId);
        hist.count = db.memberCount(p.chatId);
        send(c, hist);

        // обновим список чатов (сбросился счётчик непрочитанных)
        sendChatList(c, me);
    }

    // ─── group management ────────────────────────────────────────────────────

    private void onAddMember(WebSocket c, Packet p) {
        String me = sessions.get(c);
        if (me == null) return;
        List<String> mem = db.members(p.chatId);
        if (!mem.contains(me))          { send(c, err("Вы не в этом чате")); return; }
        if (!db.userExists(p.target))   { send(c, err("Пользователь не найден")); return; }
        if (mem.contains(p.target))     { send(c, err(p.target + " уже в чате")); return; }

        db.addMember(p.chatId, p.target);
        send(c, info(p.target + " добавлен в чат"));
        // обновим списки всем участникам
        for (String m : db.members(p.chatId)) pushChatListTo(m);
    }

    private void onLeaveChat(WebSocket c, Packet p) {
        String me = sessions.get(c);
        if (me == null) return;
        db.removeMember(p.chatId, me);
        send(c, info("Вы покинули чат"));
        sendChatList(c, me);
    }

    private void onMembers(WebSocket c, Packet p) {
        Packet resp = new Packet(PacketType.MEMBERS);
        resp.chatId = p.chatId;
        resp.members = db.members(p.chatId);
        Set<String> online = onlineSet();
        // онлайн-логины передадим отдельным списком
        resp.online = resp.members.stream().filter(online::contains).toList();
        send(c, resp);
    }

    // ─── messaging ─────────────────────────────────────────────────────────

    private void onMessage(WebSocket c, Packet p) {
        String me = sessions.get(c);
        if (me == null) return;
        List<String> mem = db.members(p.chatId);
        if (!mem.contains(me)) return;

        MessageDTO saved = db.saveMessage(p.chatId, me, p.text);
        if (saved == null) return;

        Packet out = new Packet(PacketType.MESSAGE);
        out.chatId = p.chatId;
        out.message = saved;
        String js = stringify(out);

        for (Map.Entry<WebSocket, String> e : sessions.entrySet()) {
            if (mem.contains(e.getValue())) {
                e.getKey().send(js);
                // получателям также обновим превью/счётчики в списке чатов
                if (!e.getValue().equals(me)) pushChatListTo(e.getValue());
            }
        }
    }

    private void onDeleteMessage(WebSocket c, Packet p) {
        String me = sessions.get(c);
        if (me == null) return;
        int chatId = db.chatOfMessage(p.messageId);
        if (chatId < 0) return;
        if (!db.deleteMessage(p.messageId, me)) { send(c, err("Можно удалять только свои сообщения")); return; }

        Packet out = new Packet(PacketType.DELETE_MESSAGE);
        out.chatId = chatId;
        out.messageId = p.messageId;
        String js = stringify(out);
        List<String> mem = db.members(chatId);
        for (Map.Entry<WebSocket, String> e : sessions.entrySet()) {
            if (mem.contains(e.getValue())) {
                e.getKey().send(js);
                pushChatListTo(e.getValue());
            }
        }
    }

    private void onTyping(WebSocket c, Packet p) {
        String me = sessions.get(c);
        if (me == null) return;
        List<String> mem = db.members(p.chatId);
        Packet out = new Packet(PacketType.TYPING);
        out.chatId = p.chatId;
        out.text = me;
        String js = stringify(out);
        for (Map.Entry<WebSocket, String> e : sessions.entrySet()) {
            if (mem.contains(e.getValue()) && !e.getValue().equals(me)) {
                e.getKey().send(js);
            }
        }
    }

    // ─── presence ─────────────────────────────────────────────────────────

    private Set<String> onlineSet() {
        return sessions.keySet().stream().map(sessions::get)
                .collect(java.util.stream.Collectors.toSet());
    }

    private void broadcastPresence() {
        Packet p = new Packet(PacketType.PRESENCE);
        p.online = new java.util.ArrayList<>(onlineSet());
        String js = stringify(p);
        for (WebSocket c : sessions.keySet()) c.send(js);
    }

    // ─── helpers ─────────────────────────────────────────────────────────

    private void sendChatList(WebSocket c, String login) {
        Packet p = new Packet(PacketType.CHAT_LIST);
        p.chats = db.userChats(login, onlineSet());
        p.query = "";
        send(c, p);
    }

    private void pushChatListTo(String login) {
        for (Map.Entry<WebSocket, String> e : sessions.entrySet()) {
            if (login.equals(e.getValue())) sendChatList(e.getKey(), login);
        }
    }

    private Packet err(String msg)  { return new Packet(PacketType.ERROR, msg); }
    private Packet info(String msg) { return new Packet(PacketType.INFO, msg); }

    private void send(WebSocket c, Packet p) { c.send(stringify(p)); }

    private String stringify(Packet p) {
        try { return json.writeValueAsString(p); }
        catch (Exception e) { throw new RuntimeException(e); }
    }
}

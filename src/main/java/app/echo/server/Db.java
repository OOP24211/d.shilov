package app.echo.server;

import app.echo.common.ChatDTO;
import app.echo.common.MessageDTO;

import java.sql.*;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

public class Db {
    private static final String URL  = "jdbc:postgresql://localhost:5432/echo_db";
    private static final String USER = "echo_user";
    private static final String PASS = "echo_pass";

    private static final DateTimeFormatter T_FMT = DateTimeFormatter.ofPattern("HH:mm");
    private static final DateTimeFormatter D_FMT = DateTimeFormatter.ofPattern("yyyy-MM-dd");

    private Connection cn;

    public Db() {
        try {
            cn = DriverManager.getConnection(URL, USER, PASS);
            System.out.println("[DB] подключено к PostgreSQL");
            initSchema();
        } catch (SQLException e) {
            System.err.println("[DB] ошибка подключения: " + e.getMessage());
            throw new RuntimeException(e);
        }
    }

    private void initSchema() throws SQLException {
        try (Statement s = cn.createStatement()) {
            s.execute("""
                CREATE TABLE IF NOT EXISTS users (
                    id         SERIAL PRIMARY KEY,
                    login      VARCHAR(50) UNIQUE NOT NULL,
                    pass_hash  VARCHAR(128) NOT NULL,
                    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
                );
            """);
            s.execute("""
                CREATE TABLE IF NOT EXISTS chats (
                    id         SERIAL PRIMARY KEY,
                    title      VARCHAR(120) NOT NULL DEFAULT '',
                    is_group   BOOLEAN NOT NULL DEFAULT FALSE,
                    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
                );
            """);
            s.execute("""
                CREATE TABLE IF NOT EXISTS members (
                    chat_id   INT REFERENCES chats(id) ON DELETE CASCADE,
                    user_id   INT REFERENCES users(id) ON DELETE CASCADE,
                    joined_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                    PRIMARY KEY (chat_id, user_id)
                );
            """);
            s.execute("""
                CREATE TABLE IF NOT EXISTS messages (
                    id        SERIAL PRIMARY KEY,
                    chat_id   INT REFERENCES chats(id) ON DELETE CASCADE,
                    sender_id INT REFERENCES users(id) ON DELETE CASCADE,
                    body      TEXT NOT NULL,
                    sent_at   TIMESTAMP DEFAULT CURRENT_TIMESTAMP
                );
            """);
            s.execute("""
                CREATE TABLE IF NOT EXISTS reads (
                    chat_id      INT REFERENCES chats(id) ON DELETE CASCADE,
                    user_id      INT REFERENCES users(id) ON DELETE CASCADE,
                    last_read_id INT NOT NULL DEFAULT 0,
                    PRIMARY KEY (chat_id, user_id)
                );
            """);
            System.out.println("[DB] схема готова");
        }
    }

    // ─── auth ───────────────────────────────────────────────────────────────

    public boolean register(String login, String passHash) {
        try (PreparedStatement p = cn.prepareStatement(
                "INSERT INTO users (login, pass_hash) VALUES (?, ?)")) {
            p.setString(1, login);
            p.setString(2, passHash);
            p.executeUpdate();
            return true;
        } catch (SQLException e) {
            return false; // логин занят
        }
    }

    public boolean auth(String login, String passHash) {
        try (PreparedStatement p = cn.prepareStatement(
                "SELECT 1 FROM users WHERE login = ? AND pass_hash = ?")) {
            p.setString(1, login);
            p.setString(2, passHash);
            return p.executeQuery().next();
        } catch (SQLException e) { e.printStackTrace(); return false; }
    }

    public boolean userExists(String login) {
        try (PreparedStatement p = cn.prepareStatement("SELECT 1 FROM users WHERE login = ?")) {
            p.setString(1, login);
            return p.executeQuery().next();
        } catch (SQLException e) { return false; }
    }

    private int userId(String login) throws SQLException {
        try (PreparedStatement p = cn.prepareStatement("SELECT id FROM users WHERE login = ?")) {
            p.setString(1, login);
            ResultSet rs = p.executeQuery();
            return rs.next() ? rs.getInt(1) : -1;
        }
    }

    // ─── search ─────────────────────────────────────────────────────────────

    public List<ChatDTO> search(String query, String myLogin, Set<String> online) {
        List<ChatDTO> out = new ArrayList<>();
        String sql = """
            SELECT id, login AS name, TRUE  AS is_user FROM users
             WHERE login ILIKE ? AND login <> ?
            UNION
            SELECT id, title AS name, FALSE AS is_user FROM chats
             WHERE title ILIKE ? AND is_group = TRUE
            ORDER BY name
        """;
        try (PreparedStatement p = cn.prepareStatement(sql)) {
            String pat = "%" + query + "%";
            p.setString(1, pat); p.setString(2, myLogin); p.setString(3, pat);
            ResultSet rs = p.executeQuery();
            while (rs.next()) {
                boolean isUser = rs.getBoolean("is_user");
                ChatDTO d = new ChatDTO(isUser ? -1 : rs.getInt("id"), rs.getString("name"));
                d.userResult = isUser;
                d.group = !isUser;
                if (isUser) d.online = online.contains(rs.getString("name"));
                out.add(d);
            }
        } catch (SQLException e) { e.printStackTrace(); }
        return out;
    }

    // ─── chats ──────────────────────────────────────────────────────────────

    public List<ChatDTO> userChats(String login, Set<String> online) {
        List<ChatDTO> out = new ArrayList<>();
        int uid;
        try { uid = userId(login); } catch (SQLException e) { return out; }
        if (uid < 0) return out;

        String sql = """
            SELECT c.id, c.is_group,
                   CASE WHEN c.is_group THEN c.title
                        ELSE COALESCE((SELECT u.login FROM users u
                                       JOIN members m2 ON u.id = m2.user_id
                                       WHERE m2.chat_id = c.id AND u.login <> ? LIMIT 1), 'Избранное')
                   END AS display,
                   (SELECT m.body FROM messages m WHERE m.chat_id = c.id ORDER BY m.id DESC LIMIT 1) AS last_body,
                   (SELECT u.login FROM messages m JOIN users u ON m.sender_id=u.id
                     WHERE m.chat_id = c.id ORDER BY m.id DESC LIMIT 1) AS last_sender,
                   COALESCE((SELECT MAX(id) FROM messages WHERE chat_id = c.id), 0) AS max_msg,
                   COALESCE((SELECT last_read_id FROM reads WHERE chat_id = c.id AND user_id = ?), 0) AS last_read
            FROM chats c
            JOIN members m ON c.id = m.chat_id
            WHERE m.user_id = ?
            ORDER BY max_msg DESC, c.id DESC
        """;
        try (PreparedStatement p = cn.prepareStatement(sql)) {
            p.setString(1, login);
            p.setInt(2, uid);
            p.setInt(3, uid);
            ResultSet rs = p.executeQuery();
            while (rs.next()) {
                ChatDTO d = new ChatDTO(rs.getInt("id"), rs.getString("display"));
                d.group = rs.getBoolean("is_group");

                String lastBody = rs.getString("last_body");
                String lastSender = rs.getString("last_sender");
                if (lastBody != null) {
                    String prefix = (d.group && lastSender != null) ? lastSender + ": " : "";
                    d.lastMessage = prefix + lastBody;
                }

                int maxMsg = rs.getInt("max_msg");
                int lastRead = rs.getInt("last_read");
                d.unread = countUnread(rs.getInt("id"), lastRead);

                if (!d.group) d.online = online.contains(d.title);
                out.add(d);
            }
        } catch (SQLException e) { e.printStackTrace(); }
        return out;
    }

    private int countUnread(int chatId, int lastRead) {
        try (PreparedStatement p = cn.prepareStatement(
                "SELECT COUNT(*) FROM messages WHERE chat_id = ? AND id > ?")) {
            p.setInt(1, chatId);
            p.setInt(2, lastRead);
            ResultSet rs = p.executeQuery();
            return rs.next() ? rs.getInt(1) : 0;
        } catch (SQLException e) { return 0; }
    }

    public int existingPrivate(String a, String b) {
        String sql = """
            SELECT m1.chat_id FROM members m1
            JOIN members m2 ON m1.chat_id = m2.chat_id
            JOIN users u1 ON m1.user_id = u1.id
            JOIN users u2 ON m2.user_id = u2.id
            JOIN chats c  ON m1.chat_id = c.id
            WHERE u1.login = ? AND u2.login = ? AND c.is_group = FALSE
        """;
        try (PreparedStatement p = cn.prepareStatement(sql)) {
            p.setString(1, a); p.setString(2, b);
            ResultSet rs = p.executeQuery();
            if (rs.next()) return rs.getInt(1);
        } catch (SQLException e) { e.printStackTrace(); }
        return -1;
    }

    public int createPrivate(String a, String b) {
        int ex = existingPrivate(a, b);
        if (ex != -1) return ex;
        try (PreparedStatement p = cn.prepareStatement(
                "INSERT INTO chats (title, is_group) VALUES ('', FALSE) RETURNING id")) {
            ResultSet rs = p.executeQuery();
            if (rs.next()) {
                int id = rs.getInt(1);
                addMember(id, a);
                addMember(id, b);
                return id;
            }
        } catch (SQLException e) { e.printStackTrace(); }
        return -1;
    }

    public int createGroup(String title, String creator) {
        try (PreparedStatement p = cn.prepareStatement(
                "INSERT INTO chats (title, is_group) VALUES (?, TRUE) RETURNING id")) {
            p.setString(1, title);
            ResultSet rs = p.executeQuery();
            if (rs.next()) {
                int id = rs.getInt(1);
                addMember(id, creator);
                return id;
            }
        } catch (SQLException e) { e.printStackTrace(); }
        return -1;
    }

    public void addMember(int chatId, String login) {
        try (PreparedStatement p = cn.prepareStatement(
                "INSERT INTO members (chat_id, user_id) " +
                "VALUES (?, (SELECT id FROM users WHERE login = ?)) ON CONFLICT DO NOTHING")) {
            p.setInt(1, chatId);
            p.setString(2, login);
            p.executeUpdate();
        } catch (SQLException e) { e.printStackTrace(); }
    }

    public void removeMember(int chatId, String login) {
        try (PreparedStatement p = cn.prepareStatement(
                "DELETE FROM members WHERE chat_id = ? AND user_id = (SELECT id FROM users WHERE login = ?)")) {
            p.setInt(1, chatId);
            p.setString(2, login);
            p.executeUpdate();
        } catch (SQLException e) { e.printStackTrace(); }
    }

    public boolean isGroup(int chatId) {
        try (PreparedStatement p = cn.prepareStatement("SELECT is_group FROM chats WHERE id = ?")) {
            p.setInt(1, chatId);
            ResultSet rs = p.executeQuery();
            return rs.next() && rs.getBoolean(1);
        } catch (SQLException e) { return false; }
    }

    public List<String> members(int chatId) {
        List<String> out = new ArrayList<>();
        try (PreparedStatement p = cn.prepareStatement(
                "SELECT u.login FROM users u JOIN members m ON u.id = m.user_id WHERE m.chat_id = ? ORDER BY u.login")) {
            p.setInt(1, chatId);
            ResultSet rs = p.executeQuery();
            while (rs.next()) out.add(rs.getString(1));
        } catch (SQLException e) { e.printStackTrace(); }
        return out;
    }

    public int memberCount(int chatId) {
        try (PreparedStatement p = cn.prepareStatement("SELECT COUNT(*) FROM members WHERE chat_id = ?")) {
            p.setInt(1, chatId);
            ResultSet rs = p.executeQuery();
            return rs.next() ? rs.getInt(1) : 0;
        } catch (SQLException e) { return 0; }
    }

    // ─── messages ─────────────────────────────────────────────────────────────

    public MessageDTO saveMessage(int chatId, String sender, String body) {
        try (PreparedStatement p = cn.prepareStatement(
                "INSERT INTO messages (chat_id, sender_id, body) " +
                "VALUES (?, (SELECT id FROM users WHERE login = ?), ?) RETURNING id, sent_at")) {
            p.setInt(1, chatId);
            p.setString(2, sender);
            p.setString(3, body);
            ResultSet rs = p.executeQuery();
            if (rs.next()) {
                long id = rs.getLong("id");
                var ts = rs.getTimestamp("sent_at").toLocalDateTime();
                return new MessageDTO(id, sender, body, ts.format(T_FMT), ts.format(D_FMT));
            }
        } catch (SQLException e) { e.printStackTrace(); }
        return null;
    }

    public List<MessageDTO> history(int chatId) {
        List<MessageDTO> out = new ArrayList<>();
        String sql = """
            SELECT m.id, u.login, m.body, m.sent_at
            FROM messages m JOIN users u ON m.sender_id = u.id
            WHERE m.chat_id = ?
            ORDER BY m.id ASC
        """;
        try (PreparedStatement p = cn.prepareStatement(sql)) {
            p.setInt(1, chatId);
            ResultSet rs = p.executeQuery();
            while (rs.next()) {
                var ts = rs.getTimestamp("sent_at").toLocalDateTime();
                out.add(new MessageDTO(rs.getLong("id"), rs.getString("login"),
                        rs.getString("body"), ts.format(T_FMT), ts.format(D_FMT)));
            }
        } catch (SQLException e) { e.printStackTrace(); }
        return out;
    }

    /** Удаляет сообщение, только если оно принадлежит этому пользователю. */
    public boolean deleteMessage(long msgId, String login) {
        try (PreparedStatement p = cn.prepareStatement(
                "DELETE FROM messages WHERE id = ? AND sender_id = (SELECT id FROM users WHERE login = ?)")) {
            p.setLong(1, msgId);
            p.setString(2, login);
            return p.executeUpdate() > 0;
        } catch (SQLException e) { e.printStackTrace(); return false; }
    }

    public int chatOfMessage(long msgId) {
        try (PreparedStatement p = cn.prepareStatement("SELECT chat_id FROM messages WHERE id = ?")) {
            p.setLong(1, msgId);
            ResultSet rs = p.executeQuery();
            return rs.next() ? rs.getInt(1) : -1;
        } catch (SQLException e) { return -1; }
    }

    public void markRead(int chatId, String login) {
        try {
            int uid = userId(login);
            if (uid < 0) return;
            try (PreparedStatement p = cn.prepareStatement(
                    "INSERT INTO reads (chat_id, user_id, last_read_id) " +
                    "VALUES (?, ?, COALESCE((SELECT MAX(id) FROM messages WHERE chat_id = ?), 0)) " +
                    "ON CONFLICT (chat_id, user_id) DO UPDATE SET last_read_id = " +
                    "COALESCE((SELECT MAX(id) FROM messages WHERE chat_id = ?), 0)")) {
                p.setInt(1, chatId);
                p.setInt(2, uid);
                p.setInt(3, chatId);
                p.setInt(4, chatId);
                p.executeUpdate();
            }
        } catch (SQLException e) { e.printStackTrace(); }
    }
}

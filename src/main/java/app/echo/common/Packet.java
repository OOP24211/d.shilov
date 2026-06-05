package app.echo.common;

import java.util.List;

/** Универсальный конверт для обмена клиент<->сервер (сериализуется в JSON). */
public class Packet {
    public PacketType type;

    // auth
    public String login;
    public String password;

    // search / chat
    public String  query;
    public int     chatId = -1;
    public String  target;   // логин пользователя для CREATE_PRIVATE / ADD_MEMBER
    public String  text;     // текст сообщения / название группы / инфо

    // payloads
    public List<ChatDTO>    chats;
    public List<MessageDTO> messages;
    public List<String>     members;
    public List<String>     online;

    // single message fields (для MESSAGE / DELETE / TYPING)
    public MessageDTO message;
    public long       messageId;

    public int count;  // напр. число участников

    public Packet() {}

    public Packet(PacketType type) { this.type = type; }

    public Packet(PacketType type, String text) {
        this.type = type;
        this.text = text;
    }
}

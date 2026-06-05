package app.echo.common;

/** Элемент списка чатов / результата поиска. */
public class ChatDTO {
    public int     id;          // id чата; -1 для пользователя, с которым чата ещё нет
    public String  title;       // отображаемое имя
    public boolean group;       // true = групповой чат
    public boolean userResult;  // true = это пользователь из поиска (а не чат)
    public String  lastMessage; // превью последнего сообщения
    public int     unread;      // кол-во непрочитанных
    public boolean online;      // для личных чатов/пользователей — онлайн ли собеседник

    public ChatDTO() {}

    public ChatDTO(int id, String title) {
        this.id = id;
        this.title = title;
    }

    @Override
    public String toString() { return title; }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof ChatDTO)) return false;
        return id == ((ChatDTO) o).id;
    }

    @Override
    public int hashCode() { return Integer.hashCode(id); }
}

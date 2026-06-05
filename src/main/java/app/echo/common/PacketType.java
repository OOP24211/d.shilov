package app.echo.common;

public enum PacketType {
    // auth
    REGISTER,
    LOGIN,
    AUTH_OK,
    AUTH_FAIL,
    LOGOUT,

    // search & chats
    SEARCH,
    CHAT_LIST,      // server -> client: список чатов пользователя
    OPEN_CHAT,      // client -> server: открыть чат (запросить историю)
    HISTORY,        // server -> client: история сообщений
    CREATE_PRIVATE, // создать личный чат с пользователем
    CREATE_GROUP,   // создать групповой чат
    CHAT_OPENED,    // server -> client: чат создан/открыт, переключиться на него

    // group management
    ADD_MEMBER,
    LEAVE_CHAT,
    MEMBERS,        // запрос/ответ списком участников

    // messaging
    MESSAGE,        // одно сообщение (в обе стороны)
    DELETE_MESSAGE,
    TYPING,         // индикатор печати

    // presence
    PRESENCE,       // server -> all: список логинов онлайн

    // generic
    INFO,           // информационное уведомление
    ERROR
}

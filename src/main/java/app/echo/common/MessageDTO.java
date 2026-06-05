package app.echo.common;

/** Одно сообщение чата. */
public class MessageDTO {
    public long    id;
    public String  sender;
    public String  body;
    public String  time;   // "HH:mm"
    public String  date;   // "yyyy-MM-dd" — для разделителей по датам
    public boolean separator; // служебный элемент-разделитель даты (sender/body не используются)

    public MessageDTO() {}

    public MessageDTO(long id, String sender, String body, String time, String date) {
        this.id = id;
        this.sender = sender;
        this.body = body;
        this.time = time;
        this.date = date;
    }

    public static MessageDTO dateSeparator(String date) {
        MessageDTO m = new MessageDTO();
        m.separator = true;
        m.date = date;
        return m;
    }
}

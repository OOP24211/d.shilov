package app.echo.client;

import app.echo.common.Packet;
import com.fasterxml.jackson.databind.ObjectMapper;
import javafx.application.Platform;
import org.java_websocket.client.WebSocketClient;
import org.java_websocket.handshake.ServerHandshake;

import java.net.URI;
import java.util.function.Consumer;

public class Net extends WebSocketClient {
    private final ObjectMapper json = new ObjectMapper();
    private Consumer<Packet> onPacket;
    private Runnable onOpen;
    private String login;

    public Net(URI uri, Consumer<Packet> onPacket) {
        super(uri);
        this.onPacket = onPacket;
    }

    public void setOnPacket(Consumer<Packet> c) { this.onPacket = c; }
    public void setOnOpen(Runnable r) { this.onOpen = r; }

    public String getLogin() { return login; }
    public void setLogin(String login) { this.login = login; }

    @Override public void onOpen(ServerHandshake h) {
        System.out.println("[NET] подключено");
        if (onOpen != null) Platform.runLater(onOpen);
    }

    @Override public void onMessage(String raw) {
        try {
            Packet p = json.readValue(raw, Packet.class);
            Platform.runLater(() -> onPacket.accept(p));
        } catch (Exception e) { e.printStackTrace(); }
    }

    @Override public void onClose(int code, String reason, boolean remote) {
        System.out.println("[NET] закрыто: " + reason);
    }

    @Override public void onError(Exception e) { e.printStackTrace(); }

    public void sendPacket(Packet p) {
        try {
            if (isOpen()) send(json.writeValueAsString(p));
        } catch (Exception e) { e.printStackTrace(); }
    }

    public void disconnect() {
        try {
            Packet p = new Packet();
            p.type = app.echo.common.PacketType.LOGOUT;
            sendPacket(p);
            close();
        } catch (Exception ignored) {}
    }
}

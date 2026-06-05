package app.echo.ui;

import app.echo.client.Net;
import app.echo.common.Packet;
import app.echo.common.PacketType;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

import java.net.URI;

public class AuthController {
    @FXML private TextField     loginField;
    @FXML private PasswordField passwordField;
    @FXML private Label         statusLabel;
    @FXML private Button        loginBtn;
    @FXML private Button        registerBtn;

    private Net net;
    private boolean connected = false;

    @FXML
    public void initialize() {
        statusLabel.setText("Подключение к серверу...");
        setButtonsEnabled(false);
        try {
            net = new Net(new URI("ws://localhost:9696"), this::onPacket);
            net.setOnOpen(() -> {
                connected = true;
                setButtonsEnabled(true);
                statusLabel.setText("");
            });
            net.connect();
        } catch (Exception e) {
            statusLabel.setText("Сервер недоступен");
        }
    }

    private void setButtonsEnabled(boolean v) {
        loginBtn.setDisable(!v);
        registerBtn.setDisable(!v);
    }

    @FXML private void onLogin()    { auth(PacketType.LOGIN); }
    @FXML private void onRegister() { auth(PacketType.REGISTER); }

    private void auth(PacketType type) {
        if (!connected) { statusLabel.setText("Нет соединения с сервером"); return; }
        String login = loginField.getText().trim();
        String pass  = passwordField.getText();
        if (login.isEmpty() || pass.isEmpty()) {
            warn("Заполните логин и пароль");
            return;
        }
        if (login.length() < 3) { warn("Логин минимум 3 символа"); return; }

        Packet p = new Packet(type);
        p.login = login;
        p.password = pass;
        net.sendPacket(p);
    }

    private void onPacket(Packet p) {
        switch (p.type) {
            case AUTH_OK -> {
                if ("LOGGED_IN".equals(p.text)) {
                    net.setLogin(loginField.getText().trim());
                    openHome();
                } else {
                    // регистрация удалась
                    statusLabel.setStyle("-fx-text-fill: #00b894;");
                    statusLabel.setText(p.text);
                }
            }
            case AUTH_FAIL -> warn(p.text);
            default -> {}
        }
    }

    private void warn(String msg) {
        statusLabel.setStyle("-fx-text-fill: #ff6b6b;");
        statusLabel.setText(msg);
    }

    private void openHome() {
        try {
            FXMLLoader loader = new FXMLLoader(getClass().getResource("/app/echo/ui/home.fxml"));
            Parent root = loader.load();
            HomeController home = loader.getController();
            home.attach(net);

            Stage stage = (Stage) loginField.getScene().getWindow();
            Scene scene = new Scene(root, 1040, 680);
            scene.getStylesheets().add(getClass().getResource("/app/echo/ui/theme.css").toExternalForm());
            stage.setScene(scene);
            stage.setResizable(true);
            stage.setMinWidth(880);
            stage.setMinHeight(560);
            stage.centerOnScreen();
        } catch (Exception e) {
            e.printStackTrace();
            warn("Ошибка загрузки: " + e.getMessage());
        }
    }
}

package app.echo.ui;

import app.echo.client.Net;
import app.echo.common.ChatDTO;
import app.echo.common.MessageDTO;
import app.echo.common.Packet;
import app.echo.common.PacketType;
import javafx.animation.PauseTransition;
import javafx.application.Platform;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.stage.Stage;
import javafx.util.Duration;

import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.*;

public class HomeController {

    @FXML private TextField        searchField;
    @FXML private ListView<ChatDTO> chatList;
    @FXML private ListView<ChatDTO> searchList;
    @FXML private StackPane        listsStack;

    @FXML private StackPane        avatarSlot;
    @FXML private Label            meLabel;
    @FXML private Label            chatTitleLabel;
    @FXML private Label            chatSubLabel;
    @FXML private VBox             messagesBox;        // содержимое чата (внутри ScrollPane)
    @FXML private ScrollPane       messagesScroll;
    @FXML private TextField        inputField;
    @FXML private Button           sendBtn;
    @FXML private HBox             chatHeaderActions;
    @FXML private Button           addMemberBtn;
    @FXML private Button           membersBtn;
    @FXML private Button           leaveBtn;
    @FXML private VBox             emptyState;
    @FXML private VBox             chatArea;

    private Net net;
    private int  currentChatId = -1;
    private boolean currentIsGroup = false;
    private final Set<String> online = new HashSet<>();
    private final List<MessageDTO> currentMessages = new ArrayList<>();

    private final DateTimeFormatter SEP = DateTimeFormatter.ofPattern("d MMMM");
    private PauseTransition typingTimer;

    // ─── init ────────────────────────────────────────────────────────────────

    public void attach(Net net) {
        this.net = net;
        this.net.setOnPacket(this::onPacket);

        meLabel.setText(net.getLogin());
        avatarSlot.getChildren().setAll(Avatars.make(net.getLogin(), 40));

        // первичный запрос списка чатов
        Packet p = new Packet(PacketType.SEARCH);
        p.query = "";
        net.sendPacket(p);

        showEmptyState(true);
    }

    @FXML
    public void initialize() {
        chatList.setCellFactory(lv -> new ChatCell(false));
        searchList.setCellFactory(lv -> new ChatCell(true));

        // поиск
        searchField.textProperty().addListener((o, ov, nv) -> {
            boolean searching = !nv.trim().isEmpty();
            searchList.setVisible(searching);
            searchList.setManaged(searching);
            chatList.setVisible(!searching);
            chatList.setManaged(!searching);
            if (net != null) {
                Packet p = new Packet(PacketType.SEARCH);
                p.query = nv;
                net.sendPacket(p);
            }
        });

        chatList.getSelectionModel().selectedItemProperty().addListener((o, ov, nv) -> {
            if (nv != null) openChat(nv);
        });

        searchList.setOnMouseClicked(e -> {
            if (e.getClickCount() == 2) {
                ChatDTO sel = searchList.getSelectionModel().getSelectedItem();
                if (sel != null) handleSearchPick(sel);
            }
        });

        inputField.setOnKeyPressed(e -> {
            if (e.getCode() == javafx.scene.input.KeyCode.ENTER) onSend();
            else sendTyping();
        });
    }

    // ─── packet routing ───────────────────────────────────────────────────────

    private void onPacket(Packet p) {
        switch (p.type) {
            case CHAT_LIST -> {
                if (p.chats == null) return;
                boolean searching = !searchField.getText().trim().isEmpty();
                if (searching && p.query != null && !p.query.isEmpty()) {
                    searchList.getItems().setAll(p.chats);
                } else {
                    ChatDTO sel = chatList.getSelectionModel().getSelectedItem();
                    chatList.getItems().setAll(p.chats);
                    if (sel != null) {
                        for (ChatDTO c : chatList.getItems())
                            if (c.id == sel.id) { chatList.getSelectionModel().select(c); break; }
                    }
                }
            }
            case CHAT_OPENED -> {
                int id = p.chatId;
                String title = p.text;
                searchField.clear();
                currentChatId = id;
                currentIsGroup = "GROUP".equals(p.query);
                chatTitleLabel.setText(title);
                showEmptyState(false);
                requestOpen(id);
            }
            case HISTORY -> {
                if (p.chatId != currentChatId) return;
                currentMessages.clear();
                if (p.messages != null) currentMessages.addAll(p.messages);
                renderMessages();
                chatSubLabel.setText(subtitleFor(p.count));
                showActions(p.count);
            }
            case MESSAGE -> {
                if (p.message != null && p.chatId == currentChatId) {
                    currentMessages.add(p.message);
                    renderMessages();
                }
            }
            case DELETE_MESSAGE -> {
                if (p.chatId == currentChatId) {
                    currentMessages.removeIf(m -> m.id == p.messageId);
                    renderMessages();
                }
            }
            case TYPING -> {
                if (p.chatId == currentChatId) showTyping(p.text);
            }
            case MEMBERS -> showMembersDialog(p.members, p.online);
            case PRESENCE -> {
                online.clear();
                if (p.online != null) online.addAll(p.online);
                chatList.refresh();
                searchList.refresh();
                updateChatSubFromPresence();
            }
            case INFO  -> toast(p.text, false);
            case ERROR -> toast(p.text, true);
            default -> {}
        }
    }

    // ─── chat opening ─────────────────────────────────────────────────────────

    private void openChat(ChatDTO chat) {
        currentChatId = chat.id;
        currentIsGroup = chat.group;
        chatTitleLabel.setText(chat.title);
        chatSubLabel.setText("загрузка…");
        requestOpen(chat.id);
        showEmptyState(false);
    }

    private void requestOpen(int chatId) {
        showEmptyState(false);
        Packet p = new Packet(PacketType.OPEN_CHAT);
        p.chatId = chatId;
        net.sendPacket(p);
    }

    private void handleSearchPick(ChatDTO sel) {
        if (sel.userResult) {
            Packet p = new Packet(PacketType.CREATE_PRIVATE);
            p.target = sel.title;
            net.sendPacket(p);
        } else {
            // существующая группа — открываем
            currentChatId = sel.id;
            currentIsGroup = true;
            chatTitleLabel.setText(sel.title);
            requestOpen(sel.id);
        }
        searchField.clear();
    }

    // ─── sending ─────────────────────────────────────────────────────────────

    @FXML
    private void onSend() {
        String text = inputField.getText().trim();
        if (text.isEmpty() || currentChatId == -1) return;
        Packet p = new Packet(PacketType.MESSAGE);
        p.chatId = currentChatId;
        p.text = text;
        net.sendPacket(p);
        inputField.clear();
    }

    private void sendTyping() {
        if (currentChatId == -1) return;
        Packet p = new Packet(PacketType.TYPING);
        p.chatId = currentChatId;
        net.sendPacket(p);
    }

    // ─── group actions ─────────────────────────────────────────────────────────

    @FXML
    private void onNewGroup() {
        TextInputDialog d = new TextInputDialog();
        d.setTitle("Новая группа");
        d.setHeaderText("Название группы:");
        d.setContentText("");
        styleDialog(d.getDialogPane());
        d.showAndWait().ifPresent(name -> {
            if (!name.trim().isEmpty()) {
                Packet p = new Packet(PacketType.CREATE_GROUP);
                p.text = name.trim();
                net.sendPacket(p);
            }
        });
    }

    @FXML
    private void onAddMember() {
        if (currentChatId == -1 || !currentIsGroup) {
            toast("Добавлять можно только в группу", true); return;
        }
        TextInputDialog d = new TextInputDialog();
        d.setTitle("Добавить участника");
        d.setHeaderText("Логин пользователя:");
        styleDialog(d.getDialogPane());
        d.showAndWait().ifPresent(login -> {
            if (!login.trim().isEmpty()) {
                Packet p = new Packet(PacketType.ADD_MEMBER);
                p.chatId = currentChatId;
                p.target = login.trim();
                net.sendPacket(p);
            }
        });
    }

    @FXML
    private void onMembers() {
        if (currentChatId == -1) return;
        Packet p = new Packet(PacketType.MEMBERS);
        p.chatId = currentChatId;
        net.sendPacket(p);
    }

    @FXML
    private void onLeave() {
        if (currentChatId == -1 || !currentIsGroup) {
            toast("Покинуть можно только группу", true); return;
        }
        Alert a = new Alert(Alert.AlertType.CONFIRMATION, "Покинуть эту группу?",
                ButtonType.OK, ButtonType.CANCEL);
        a.setHeaderText(null);
        styleDialog(a.getDialogPane());
        a.showAndWait().ifPresent(b -> {
            if (b == ButtonType.OK) {
                Packet p = new Packet(PacketType.LEAVE_CHAT);
                p.chatId = currentChatId;
                net.sendPacket(p);
                currentChatId = -1;
                showEmptyState(true);
            }
        });
    }

    @FXML
    private void onLogout() {
        Alert a = new Alert(Alert.AlertType.CONFIRMATION, "Выйти из аккаунта?",
                ButtonType.OK, ButtonType.CANCEL);
        a.setHeaderText(null);
        styleDialog(a.getDialogPane());
        a.showAndWait().ifPresent(b -> {
            if (b == ButtonType.OK) {
                net.disconnect();
                try {
                    Stage stage = (Stage) chatList.getScene().getWindow();
                    FXMLLoader loader = new FXMLLoader(getClass().getResource("/app/echo/ui/auth.fxml"));
                    Scene scene = new Scene(loader.load(), 440, 520);
                    scene.getStylesheets().add(getClass().getResource("/app/echo/ui/theme.css").toExternalForm());
                    stage.setScene(scene);
                    stage.setResizable(false);
                    stage.centerOnScreen();
                } catch (Exception e) { e.printStackTrace(); }
            }
        });
    }

    // ─── rendering messages ────────────────────────────────────────────────────

    private void renderMessages() {
        messagesBox.getChildren().clear();
        String me = net.getLogin();
        String prevDate = null;

        for (MessageDTO m : currentMessages) {
            if (!Objects.equals(prevDate, m.date)) {
                messagesBox.getChildren().add(dateSeparator(m.date));
                prevDate = m.date;
            }
            messagesBox.getChildren().add(bubbleRow(m, m.sender.equals(me)));
        }
        Platform.runLater(() -> messagesScroll.setVvalue(1.0));
    }

    private Node dateSeparator(String isoDate) {
        String label = isoDate;
        try { label = LocalDate.parse(isoDate).format(SEP); } catch (Exception ignored) {}
        Label l = new Label(label);
        l.getStyleClass().add("date-sep");
        HBox box = new HBox(l);
        box.setAlignment(Pos.CENTER);
        box.setPadding(new Insets(10, 0, 6, 0));
        return box;
    }

    private Node bubbleRow(MessageDTO m, boolean mine) {
        VBox bubble = new VBox(2);
        bubble.getStyleClass().addAll("bubble", mine ? "bubble-mine" : "bubble-other");
        bubble.setMaxWidth(480);

        if (!mine && currentIsGroup) {
            Label sender = new Label(m.sender);
            sender.getStyleClass().add("bubble-sender");
            sender.setStyle("-fx-text-fill: " + toHex(Avatars.colorFor(m.sender)) + ";");
            bubble.getChildren().add(sender);
        }

        Label body = new Label(m.body);
        body.setWrapText(true);
        body.setMaxWidth(440);
        body.getStyleClass().add("bubble-body");

        Label time = new Label(m.time);
        time.getStyleClass().add("bubble-time");
        HBox meta = new HBox(time);
        meta.setAlignment(Pos.CENTER_RIGHT);

        bubble.getChildren().addAll(body, meta);

        // контекстное меню для удаления своих сообщений
        if (mine) {
            ContextMenu menu = new ContextMenu();
            MenuItem del = new MenuItem("Удалить");
            del.setOnAction(e -> {
                Packet p = new Packet(PacketType.DELETE_MESSAGE);
                p.messageId = m.id;
                net.sendPacket(p);
            });
            menu.getItems().add(del);
            bubble.setOnContextMenuRequested(e -> menu.show(bubble, e.getScreenX(), e.getScreenY()));
        }

        HBox row = new HBox();
        row.setPadding(new Insets(2, 12, 2, 12));
        row.setAlignment(mine ? Pos.CENTER_RIGHT : Pos.CENTER_LEFT);

        if (!mine) {
            StackPane av = Avatars.make(m.sender, 32);
            HBox.setMargin(av, new Insets(0, 8, 0, 0));
            row.getChildren().addAll(av, bubble);
        } else {
            row.getChildren().add(bubble);
        }
        return row;
    }

    // ─── typing indicator ─────────────────────────────────────────────────────

    private void showTyping(String who) {
        chatSubLabel.setText(who + " печатает…");
        if (typingTimer != null) typingTimer.stop();
        typingTimer = new PauseTransition(Duration.seconds(2.5));
        typingTimer.setOnFinished(e -> updateChatSubFromPresence());
        typingTimer.playFromStart();
    }

    private void updateChatSubFromPresence() {
        if (currentChatId == -1) return;
        if (currentIsGroup) return; // для групп оставляем число участников
        ChatDTO c = chatList.getItems().stream()
                .filter(x -> x.id == currentChatId).findFirst().orElse(null);
        String title = c != null ? c.title : chatTitleLabel.getText();
        chatSubLabel.setText(online.contains(title) ? "в сети" : "не в сети");
    }

    private String subtitleFor(int memberCount) {
        if (currentIsGroup) return memberCount + " участник(ов)";
        return online.contains(chatTitleLabel.getText()) ? "в сети" : "не в сети";
    }

    // ─── members dialog ─────────────────────────────────────────────────────────

    private void showMembersDialog(List<String> members, List<String> onlineMembers) {
        if (members == null) return;
        Set<String> on = onlineMembers == null ? Set.of() : new HashSet<>(onlineMembers);

        VBox content = new VBox(8);
        content.setPadding(new Insets(4));
        for (String m : members) {
            HBox row = new HBox(10);
            row.setAlignment(Pos.CENTER_LEFT);
            StackPane av = Avatars.make(m, 30);
            Label name = new Label(m);
            name.setStyle("-fx-text-fill: #e8eaed; -fx-font-size: 14px;");
            Circle dot = new Circle(5, on.contains(m) ? Color.web("#2ecc71") : Color.web("#555a66"));
            row.getChildren().addAll(av, name, dot);
            content.getChildren().add(row);
        }
        Alert a = new Alert(Alert.AlertType.NONE);
        a.setTitle("Участники");
        a.setHeaderText("Участники (" + members.size() + ")");
        a.getDialogPane().setContent(content);
        a.getButtonTypes().add(ButtonType.CLOSE);
        styleDialog(a.getDialogPane());
        a.show();
    }

    // ─── state helpers ─────────────────────────────────────────────────────────

    private void showActions(int memberCount) {
        boolean group = currentIsGroup;
        addMemberBtn.setVisible(group); addMemberBtn.setManaged(group);
        leaveBtn.setVisible(group);     leaveBtn.setManaged(group);
        membersBtn.setVisible(true);    membersBtn.setManaged(true);
    }

    private void showEmptyState(boolean empty) {
        emptyState.setVisible(empty); emptyState.setManaged(empty);
        chatArea.setVisible(!empty);  chatArea.setManaged(!empty);
    }

    private void toast(String msg, boolean error) {
        if (msg == null) return;
        if ("REGISTERED".equals(msg)) return;
        Label l = new Label(msg);
        l.getStyleClass().add(error ? "toast-error" : "toast-info");
        StackPane.setAlignment(l, Pos.TOP_CENTER);
        StackPane.setMargin(l, new Insets(16, 0, 0, 0));
        if (rootStack() != null) {
            rootStack().getChildren().add(l);
            PauseTransition pt = new PauseTransition(Duration.seconds(2.2));
            pt.setOnFinished(e -> rootStack().getChildren().remove(l));
            pt.play();
        }
    }

    private StackPane rootStack() {
        if (chatList.getScene() == null) return null;
        Parent root = chatList.getScene().getRoot();
        return root instanceof StackPane ? (StackPane) root : null;
    }

    private void styleDialog(DialogPane pane) {
        pane.getStylesheets().add(getClass().getResource("/app/echo/ui/theme.css").toExternalForm());
        pane.getStyleClass().add("echo-dialog");
    }

    private static String toHex(Color c) {
        return String.format("#%02x%02x%02x",
                (int) (c.getRed() * 255), (int) (c.getGreen() * 255), (int) (c.getBlue() * 255));
    }

    // ─── chat list cell ────────────────────────────────────────────────────────

    private class ChatCell extends ListCell<ChatDTO> {
        private final boolean searchMode;
        ChatCell(boolean searchMode) { this.searchMode = searchMode; }

        @Override
        protected void updateItem(ChatDTO item, boolean empty) {
            super.updateItem(item, empty);
            if (empty || item == null) { setGraphic(null); setText(null); return; }

            StackPane avatarPane = Avatars.make(item.title, 46);

            // онлайн-точка для личных чатов/пользователей
            boolean showOnline = (!item.group) && item.online;
            if (showOnline || (item.userResult && online.contains(item.title))) {
                Circle dot = new Circle(6, Color.web("#2ecc71"));
                dot.setStroke(Color.web("#171923"));
                dot.setStrokeWidth(2);
                StackPane.setAlignment(dot, Pos.BOTTOM_RIGHT);
                avatarPane.getChildren().add(dot);
            }

            Label title = new Label(item.title);
            title.getStyleClass().add("chat-title");

            String sub = searchMode
                    ? (item.userResult ? "пользователь" : "группа")
                    : (item.lastMessage != null ? item.lastMessage : "нет сообщений");
            Label subtitle = new Label(sub);
            subtitle.getStyleClass().add("chat-sub");
            subtitle.setMaxWidth(170);

            VBox texts = new VBox(2, title, subtitle);
            texts.setAlignment(Pos.CENTER_LEFT);
            HBox.setHgrow(texts, Priority.ALWAYS);

            HBox row = new HBox(12, avatarPane, texts);
            row.setAlignment(Pos.CENTER_LEFT);
            row.setPadding(new Insets(8, 6, 8, 6));

            // бейдж непрочитанных
            if (!searchMode && item.unread > 0) {
                Label badge = new Label(String.valueOf(item.unread));
                badge.getStyleClass().add("unread-badge");
                row.getChildren().add(badge);
            }

            setGraphic(row);
            setText(null);
        }
    }
}

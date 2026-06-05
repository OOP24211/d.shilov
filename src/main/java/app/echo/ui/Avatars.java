package app.echo.ui;

import javafx.scene.layout.StackPane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.control.Label;

/** Круглый аватар с инициалом и цветом, выведенным из логина. */
public final class Avatars {
    private Avatars() {}

    private static final Color[] PALETTE = {
        Color.web("#7c6cf0"), Color.web("#e84393"), Color.web("#00b894"),
        Color.web("#0984e3"), Color.web("#fdcb6e"), Color.web("#e17055"),
        Color.web("#6c5ce7"), Color.web("#00cec9"), Color.web("#fd79a8"),
        Color.web("#55efc4"), Color.web("#fab1a0"), Color.web("#a29bfe")
    };

    public static Color colorFor(String login) {
        if (login == null || login.isEmpty()) return PALETTE[0];
        int h = Math.abs(login.hashCode());
        return PALETTE[h % PALETTE.length];
    }

    public static StackPane make(String login, double size) {
        Circle bg = new Circle(size / 2);
        bg.setFill(colorFor(login));

        String initial = (login == null || login.isEmpty())
                ? "?" : login.substring(0, 1).toUpperCase();
        Label lbl = new Label(initial);
        lbl.setStyle("-fx-text-fill: white; -fx-font-weight: bold; -fx-font-size: " + (size * 0.42) + "px;");

        StackPane pane = new StackPane(bg, lbl);
        pane.setMinSize(size, size);
        pane.setMaxSize(size, size);
        pane.setPrefSize(size, size);
        return pane;
    }
}

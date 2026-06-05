package app.echo.ui;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class App extends Application {
    @Override
    public void start(Stage stage) throws Exception {
        FXMLLoader loader = new FXMLLoader(App.class.getResource("/app/echo/ui/auth.fxml"));
        Scene scene = new Scene(loader.load(), 440, 520);
        scene.getStylesheets().add(App.class.getResource("/app/echo/ui/theme.css").toExternalForm());
        stage.setTitle("Echo");
        stage.setScene(scene);
        stage.setResizable(false);
        stage.show();
    }

    public static void main(String[] args) { launch(); }
}

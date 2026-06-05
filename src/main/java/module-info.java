module echo {
    requires javafx.controls;
    requires javafx.fxml;

    requires com.fasterxml.jackson.databind;
    requires com.fasterxml.jackson.core;
    requires org.java_websocket;
    requires java.sql;
    requires org.slf4j;

    opens app.echo.ui     to javafx.fxml;
    opens app.echo.common to com.fasterxml.jackson.databind;

    exports app.echo.ui;
    exports app.echo.common;
    exports app.echo.client;
    exports app.echo.server;
}

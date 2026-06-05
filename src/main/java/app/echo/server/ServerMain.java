package app.echo.server;

public class ServerMain {
    public static void main(String[] args) {
        int port = 9696;
        try {
            Server server = new Server(port);
            server.start();
            System.out.println("Echo сервер запущен на порту " + port);
        } catch (Exception e) {
            System.err.println("Не удалось запустить сервер: " + e.getMessage());
            e.printStackTrace();
        }
    }
}

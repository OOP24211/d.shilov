package app.echo.server;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/** Простое хеширование паролей (SHA-256). Для учебного проекта достаточно. */
public final class Security {
    private Security() {}

    public static String hash(String password) {
        try {
            MessageDigest md = MessageDigest.getInstance("SHA-256");
            byte[] digest = md.digest(("echo$" + password).getBytes(java.nio.charset.StandardCharsets.UTF_8));
            StringBuilder sb = new StringBuilder();
            for (byte b : digest) sb.append(String.format("%02x", b));
            return sb.toString();
        } catch (NoSuchAlgorithmException e) {
            throw new RuntimeException(e);
        }
    }
}

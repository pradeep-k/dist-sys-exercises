// CS 6421 - Simple Message Board Client in Java
// T. Wood
// Compile with: javac MsgClient
// Run with:     java MsgClient

import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class MsgClient {
    public static void main(String[] args) {
        //String host = "twood02.koding.io";
        String host = "127.0.0.1";
        int portnum = 5555;
        String name = "Pradeep";
        String message = "Hello";

        // Your code here!
        try {
            Socket sock = new Socket(host, portnum);
            
            PrintWriter out = new PrintWriter(sock.getOutputStream(), true);
            out.println(name);
            out.println(message);
        } catch (UnknownHostException e) {
        } catch (IOException e) {
        }
        

    }
}

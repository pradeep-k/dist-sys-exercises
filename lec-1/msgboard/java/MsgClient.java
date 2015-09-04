// CS 6421 - Simple Message Board Client in Java
// T. Wood
// Compile with: javac MsgClient
// Run with:     java MsgClient

import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import gnu.getopt.Getopt;

public class MsgClient {
    public static void usage() {
       System.out.print("java MsgClient -h for help\n");
       System.out.print("java MsgClient -s <server> -p <port> -n <name> -m <message> \n");

    }
    public static void main(String[] args) {
        //String host = "twood02.koding.io";
        String host = "127.0.0.1";
        int portnum = 5555;
        String name = "Pradeep";
        String message = "Hello";

        Getopt g = new Getopt("MsgClient", args, "p:h:n:m:m:s:");
        int o;
        while ((o = g.getopt()) != -1) {
                switch(o){
                case 'p':
                        portnum = Integer.parseInt(g.getOptarg());
                        break;
                case 's':
                        host = g.getOptarg();
                        break;
                case 'm':
                        message = g.getOptarg();
                        break;
                case 'n':
                        name = g.getOptarg();
                        break;
                case 'h':
                        usage();
                        return ;
                case '?':
                        
                        if (g.getOptopt() == 'h') { 
                                usage();
                                return ;
                        } else if(g.getOptopt() == 'p' || 
                                  g.getOptopt() == 's' || 
                                  g.getOptopt() == 'm' || 
                                  g.getOptopt() == 'n' ) {
                                System.out.println("Option" +  
                                                 g.getOptopt() +
                                                 "requires an argument"); 
                         
                        } else {
                                System.out.println("Unknown argument:" +  
                                                 g.getOptopt());
                        }
                        return ;
                }
        }
        // Your code here!
        try {
                Socket sock = new Socket(host, portnum);
            
                PrintWriter out = new PrintWriter(sock.getOutputStream(), true);
                out.println(name);
                out.println(message);
        } catch (UnknownHostException e) {
                System.out.print("UnknownHost Exception .\n");
        } catch (IOException e) {
                System.out.print("IO Exception .\n");
        }
        

    }
}

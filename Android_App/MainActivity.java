package chouf.WCR;

import java.io.BufferedWriter;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import android.app.Activity;
import android.os.Bundle;
//import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity {
    /** Called when the activity is first created. */
	private boolean connected = false;
	private boolean ActionDecidee=false;
	//private Handler handler = new Handler();
	private String ActionDemandee="Arriere";
	private String serverIpAddress = "192.168.1.20";
	private int serverPort=1080;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        Thread cThread = new Thread(new ClientThread());
        cThread.start();
    }
    
    private void setButtonClickListener(){
    	
    	Button AvantButton = (Button)findViewById(R.id.AvantButton);
    	Button AvtGchButton = (Button)findViewById(R.id.avtgchbutton);
    	Button AvtDteButton = (Button)findViewById(R.id.AvtDteButton);
    	Button ArretButton = (Button)findViewById(R.id.ArretButton);
    	Button ArrGchButton = (Button)findViewById(R.id.ArrGchButton);
    	Button ArriereButton = (Button)findViewById(R.id.ArriereButton);
    	Button ArrDteButton = (Button)findViewById(R.id.ArrDteButton);
    	Button FinButton = (Button)findViewById(R.id.FinButton);
    	
    	AvantButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			ActionDemandee="Avant";
    			ActionDecidee=true;
    		}
    	});
    	
    	AvtGchButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			ActionDemandee="AvtGche";
    			ActionDecidee=true;
    		}
    	});
    	
    	AvtDteButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			ActionDemandee="AvtDte";
    			ActionDecidee=true;
    		}
    	});
    	
    	ArretButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			ActionDemandee="Arret";
    			ActionDecidee=true;
    		}
    	});
    	
    	ArriereButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			ActionDemandee="Arriere";
    			ActionDecidee=true;
    		}
    	});
    	
    	ArrGchButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			ActionDemandee="ArrGche";
    			ActionDecidee=true;
    		}
    	});
    	
    	ArrDteButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			ActionDemandee="ArrDte";
    			ActionDecidee=true;
    		}
    	});
    	
    	FinButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			ActionDemandee="Fin";
    			ActionDecidee=true;
    		}
    	});
    }
    
    public class ClientThread implements Runnable {
     
    	    public void run() {
    	           try {
    	        	   InetAddress serverAddr = InetAddress.getByName(serverIpAddress);
    	        	   Socket socket = new Socket(serverAddr, serverPort);
    	        	   connected = true;
    	           		
    	        	   try {
   	                    PrintWriter out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket
   	                                    .getOutputStream())), true);
   	                    //out.println("Debug");
   	    	            while (connected) {  
   		                    setButtonClickListener();
   	    	                if (ActionDecidee){
   	    	    	              out.println(ActionDemandee);
   	    	                      ActionDecidee=false;
   	    	                }
   	    	            }
       	            	} catch (Exception e) {
       	            		Log.e("ClientActivity", "S: Error", e);
   		                }
		                socket.close();
    	           } catch (Exception e) {
    	           			Log.e("ClientActivity", "C: Error", e);
    	           			connected = false;
    	           		}

                    
    	                    
    		            }
    		                //}
    		            
    		        }
    		    }

package com.example.tomydear;

public class NetThread extends Thread {
	private NetCore netCore;
	@Override
	public void run() {
		netCore.Init();
		netCore.Connect("123.57.2.139", (char)8888);
		// TODO Auto-generated method stub
		//super.run();
		while (!Thread.currentThread().isInterrupted()) {  
			
            try {  
            	netCore.Recv();
                Thread.sleep(1);  
            } catch (InterruptedException e) {  
                e.printStackTrace();  
            }  
        }  
	}

}

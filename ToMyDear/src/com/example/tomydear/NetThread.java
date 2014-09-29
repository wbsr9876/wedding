package com.example.tomydear;

public class NetThread extends Thread {
	private NetCore netCore = new NetCore();
	@Override
	public void run() {
		int temp = 0;
		temp = netCore.Init();
		if(temp != 0) return;
		temp = netCore.Connect("123.57.2.139", (char)8888);
		if(temp != 0) return;
		// TODO Auto-generated method stub
		//super.run();
		while (!Thread.currentThread().isInterrupted()) {  
			
            try {  
            	byte [] data = new byte[128];
            	netCore.Recv(data);
                Thread.sleep(1);  
            } catch (InterruptedException e) {  
                e.printStackTrace();  
            }  
        }  
	}

}

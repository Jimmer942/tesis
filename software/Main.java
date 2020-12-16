import com.panamahitek.ArduinoException;
import com.panamahitek.PanamaHitek_Arduino;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;


public class Main {

	public static DefaultTableModel model = new DefaultTableModel();
	private static PanamaHitek_Arduino ino = new PanamaHitek_Arduino();
	private static final SerialPortEventListener listener=new SerialPortEventListener(){
		String[] row = new String[3];
		@Override public void serialEvent(SerialPortEvent spe){
			try{
				if(ino.isMessageAvailable()){
					// Se imprime el mensaje recibido en la consola
					String msg = ino.printMessage();			
					if (msg.charAt(0) == '~'){
						String id = msg.substring(1);
						row[1] = id;
						System.out.println(row[0]);
					}
					if (msg.charAt(0) == '*'){
						String est = msg.substring(1,2);
						String tim = msg.substring(2);

						row[0] = est;
						row[2] = tim;
						System.out.println("Inicio");
						for(int i = 0; i < row.length; i++){
							System.out.print(row[i] + " ");
						}
						model.addRow(row);
						System.out.println();
						System.out.println("Fin");			
					}
				}
			}catch(SerialPortException|ArduinoException ex)
			{
				System.out.println(ex.getMessage());
				return;
			}
		}
	};

	public static void main(String []args) {
		/*Scanner sc = new Scanner(System.in);
		String portName = sc.next();*/
		JFrame f = new JFrame();
		JTable jt = new JTable(model);
		model.addColumn("Estacion");
		model.addColumn("Identificador de producto");
		model.addColumn("Tiempo total");
		jt.setBounds(30,40,200,300);
		JScrollPane sp=new JScrollPane(jt);
		f.add(sp);
		f.setSize(500,500);
		f.setVisible(true);
		String port = "/dev/ttyACM0";
		try {
			ino.arduinoRX(port, 9600, listener);
		} catch (ArduinoException | SerialPortException ex) {
			System.out.println(ex.getMessage());
			return;
		}
	}
}

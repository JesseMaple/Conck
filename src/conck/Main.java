package conck;

import conck.ui.ConsoleUI;
import conck.repository.ContactRepository;
import conck.repository.FileContactRepository;
import conck.service.ContactService;

public class Main {
    public static void main(String[] args) {
    	String filepath="contacts.txt";
    	ContactRepository repo=new FileContactRepository(filepath);
    	ContactService service=new ContactService(repo);
        ConsoleUI ui = new ConsoleUI(service);
        ui.run();
    }
}
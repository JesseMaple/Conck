package conck.ui;

import java.util.Scanner;
import conck.service.ContactService;
import conck.model.Contact;
import java.util.List;

public class ConsoleUI {
    private final Scanner sc = new Scanner(System.in);
    private ContactService service;
    
    public ConsoleUI(ContactService service) {this.service=service;}

    public void run() {
        while (true) {
            mainMenu();
            String input = sc.nextLine().trim();

            switch (input) {
                case "look up" -> doRetrieve();
                case "add"     -> doCreate();
                case "modify"  -> doModify();
                case "delete"  -> doDelete();
                case "quit"    -> {
                    successfulOp();
                    sc.close();
                    return;          // 直接结束 run()，等价于你原来的 break MAIN
                }
                default -> invalidToken();
            }
        }
    }

    // ---------- 各操作的流程 ----------

    private void doRetrieve() {
        while(true) {
        	displayContacts(service.getContacts());
        	retrieveMenu();
        	String input=sc.nextLine().trim().toLowerCase();
        	switch(input) {
        	case "return":return;
        	case "name":{
        		displayContacts(service.rearrangeByName());
        		sc.nextLine();
        		break;
        	}
        	case "number":{
        		displayContacts(service.rearrangeByNumber());
        		sc.nextLine();
        		break;
        	}
        	default:{
        		displayContacts(service.search(input));
        		sc.nextLine();
        		break;
        	}
        	}
        }
    }

    private void doCreate() {
        while(true) {
        	createMenu();
        	String name=null;
        	String number=null;
        	for(int i=0;i<2;i++) {
        		String input=sc.nextLine().trim();
        		if(input.equals("return")) {
        			return;
        		}
        		if(i==0) {
        			name=input;
        		}
        		else {
        			number=input;
        		}
        	}
        	if(service.add(name,number)) {
        		successfulOp();
        	}
        	else {
        		System.out.println("Aready exists");
        	}
        }
    }

    private void doModify() {
    	while (true) {
            modifyMenu();
            String input = sc.nextLine().trim();
            if (input.equals("return")) {
                return;
            }

            List<Contact> matched = service.search(input);
            if (matched.isEmpty()) {
                System.out.println("Not found");
                continue;
            }
            displayContacts(matched);

            String which = sc.nextLine().trim();
            if (which.equals("return")) {
                continue;
            }

            Contact target = null;
            for (Contact c : matched) {
                if (c.name().equals(which) || c.number().equals(which)) {
                    target = c;
                    break;
                }
            }
            if (target == null) {
                System.out.println("Not found");
                continue;
            }

            String field = sc.nextLine().trim();
            if (field.equals("return")) {
                continue;
            }

            String value = sc.nextLine().trim();
            if (value.equals("return")) {
                continue;
            }

            boolean ok;
            if (field.equals("name")) {
                ok = service.update(target, value, target.number());
            } else if (field.equals("number")) {
                ok = service.update(target, target.name(), value);
            } else {
                invalidToken();
                continue;
            }

            if (ok) {
                successfulOp();
            } else {
                System.out.println("Failed");
            }
        }
    }

    private void doDelete() {
    	while (true) {
            deleteMenu();
            String input = sc.nextLine().trim();
            if (input.equals("return")) {
                return;
            }

            List<Contact> matched = service.search(input);
            if (matched.isEmpty()) {
                System.out.println("Not found");
                continue;
            }
            displayContacts(matched);

            String which = sc.nextLine().trim();
            if (which.equals("return")) {
                continue;
            }

            Contact target = null;
            for (Contact c : matched) {
                if (c.name().equals(which) || c.number().equals(which)) {
                    target = c;
                    break;
                }
            }
            if (target == null) {
                System.out.println("Not found");
                continue;
            }

            String confirm = sc.nextLine().trim();
            if (confirm.equals("yes")) {
                if (service.remove(target)) {
                    successfulOp();
                } else {
                    System.out.println("Failed");
                }
            } else {
                System.out.println("Cancelled");
            }
        }
    }

    // ---------- 显示 ----------

    private void mainMenu() {
        System.out.println("""
                look up --find who you want
                add	--add who you input
                modify --change informations of whom you choose
                delete --delete who you choose
                quit --exit this program
                """);
    }
    
    private void createMenu() {
    	System.out.println("""
    			return --back to last page
    			<any name> --record name
    			<any number> --record number
    			"""
    			);
    }
    
    private void retrieveMenu() {
    	System.out.println("""
    			return --back to last page
    			name --rerange contacts by name
    			number --rerange contacts by number
    			<any name or number> --serch a contact
    			"""
    			);
    }
    
    private void modifyMenu() {
        System.out.println("""
                return --back to last page
                <any name or number> --search a contact
                <which one> --choose one from the list
                name / number --field to change
                <new value> --new name or number
                """);
    }

    private void deleteMenu() {
        System.out.println("""
                return --back to last page
                <any name or number> --search a contact
                <which one> --choose one from the list
                yes / no --confirm delete
                """);
    }

    private void successfulOp() {
        System.out.println("Successful operation");
    }

    private void invalidToken() {
        System.out.println("Invalid token");
    }
    
    private void displayContacts(List<Contact> list) {
    	for(Contact contact:list) {
    		System.out.println(contact.toString());
    	}
    }
}
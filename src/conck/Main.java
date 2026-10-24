package conck;

import conck.model.Contact;
import conck.repository.ContactRepository;
import conck.repository.FileContactRepository;
import conck.service.ContactService;
import conck.ui.ConsoleUI;

import java.util.List;

// E:\ProEnvironments\JavaEn\Java25\bin\jpackage.exe --type app-image --name Conck --input . --main-jar conck.jar --main-class conck.Main --win-console --dest output
// E:\ProEnvironments\JavaEn\Java25\bin\jpackage.exe --type app-image --name Conck --input . --main-jar conck.jar --main-class conck.Main --win-console --dest output

public class Main {
    public static void main(String[] args) {
        // 1. 组装数据层与业务层（组装是主体的事）
        ContactRepository repo = new FileContactRepository("contacts.json");
        ContactService service = new ContactService(repo);

        // 2. 创建纯 I/O 层
        ConsoleUI ui = new ConsoleUI();

        // 3. 主体驱动业务流程
        runMainLoop(service, ui);
    }

    private static void runMainLoop(ContactService service, ConsoleUI ui) {
        while (true) {
            ui.showMainMenu();
            String cmd = ui.readLine();
            switch (cmd) {
                case "rv" -> doRetrieve(service, ui);
                case "ct" -> doCreate(service, ui);
                case "ud" -> doModify(service, ui);
                case "dl" -> doDelete(service, ui);
                case "qt" -> { ui.showMessage("Bye."); return; }
                default   -> ui.showMessage("Invalid command");
            }
        }
    }

    private static void doRetrieve(ContactService service, ConsoleUI ui) {
        while (true) {
            ui.showContacts(service.getContacts());
            ui.showRetrieveMenu();
            String cmd = ui.readLine();
            switch (cmd) {
                case "rt" -> { return; }
                case "nm"   -> ui.showContacts(service.rearrangeByName());
                case "nb" -> ui.showContacts(service.rearrangeByNumber());
                default       -> ui.showContacts(service.search(cmd));
            }
        }
    }

    private static void doCreate(ContactService service, ConsoleUI ui) {
        while (true) {
            ui.showCreateMenu();
            String name = ui.readLine();
            if (name.equals("rt")) return;
            String number = ui.readLine();
            if (number.equals("rt")) return;

            if (service.add(name, number)) {
                ui.showMessage("Added.");
            } else {
                ui.showMessage("Contact already exists.");
            }
        }
    }

    private static void doModify(ContactService service, ConsoleUI ui) {
        while (true) {
            ui.showModifyMenu();
            String query = ui.readLine();
            if (query.equals("rt")) return;

            List<Contact> matched = service.search(query);
            if (matched.isEmpty()) {
                ui.showMessage("Not found.");
                continue;
            }
            ui.showContacts(matched);

            String which = ui.readLine();
            if (which.equals("rt")) continue;

            Contact target = pick(matched, which);
            if (target == null) {
                ui.showMessage("Not found.");
                continue;
            }

            String field = ui.readLine();
            if (field.equals("rt")) continue;
            String value = ui.readLine();
            if (value.equals("rt")) continue;

            boolean ok;
            if (field.equals("nm")) {
                ok = service.update(target, value, target.number());
            } else if (field.equals("nb")) {
                ok = service.update(target, target.name(), value);
            } else {
                ui.showMessage("Invalid field.");
                continue;
            }
            ui.showMessage(ok ? "Updated." : "Update failed.");
        }
    }

    private static void doDelete(ContactService service, ConsoleUI ui) {
        while (true) {
            ui.showDeleteMenu();
            String query = ui.readLine();
            if (query.equals("rt")) return;

            List<Contact> matched = service.search(query);
            if (matched.isEmpty()) {
                ui.showMessage("Not found.");
                continue;
            }
            ui.showContacts(matched);

            String which = ui.readLine();
            if (which.equals("rt")) continue;

            Contact target = pick(matched, which);
            if (target == null) {
                ui.showMessage("Not found.");
                continue;
            }

            String confirm = ui.readLine();
            if (confirm.equals("yes")) {
                ui.showMessage(service.remove(target) ? "Deleted." : "Delete failed.");
            } else {
                ui.showMessage("Cancelled.");
            }
        }
    }

    private static Contact pick(List<Contact> list, String key) {
        for (Contact c : list) {
            if (c.name().equals(key) || c.number().equals(key)) return c;
        }
        return null;
    }
}
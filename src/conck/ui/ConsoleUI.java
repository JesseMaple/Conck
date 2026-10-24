package conck.ui;

import conck.model.Contact;

import java.util.List;
import java.util.Scanner;

public class ConsoleUI {
    // 表格列宽（按终端显示宽度计算，可自行调整）
    private static final int ID_WIDTH = 4;
    private static final int NAME_WIDTH = 24;
    private static final int NUMBER_WIDTH = 20;

    private final Scanner scanner = new Scanner(System.in);
    
    /** 读取一行去掉首尾空格的输入 */
    public String readLine() {
        return scanner.nextLine().trim();
    }

    /** 输出一条消息 */
    public void showMessage(String message) {
        System.out.println(message);
    }

    /** 输出联系人列表（表格化） */
    public void showContacts(List<Contact> contacts) {
        if (contacts.isEmpty()) {
            System.out.println("(no contacts)");
            return;
        }

        System.out.println(formatRow("ID", "Name", "Number"));
        System.out.println(separator());

        int id = 1;
        for (Contact c : contacts) {
            System.out.println(formatRow(String.valueOf(id++), c.name(), c.number()));
        }
    }

    private static String separator() {
        return "-".repeat(ID_WIDTH) + "-+-"
                + "-".repeat(NAME_WIDTH) + "-+-"
                + "-".repeat(NUMBER_WIDTH);
    }

    private static String formatRow(String id, String name, String number) {
        return padRight(id, ID_WIDTH) + " | "
                + padRight(sanitize(name), NAME_WIDTH) + " | "
                + padRight(sanitize(number), NUMBER_WIDTH);
    }

    /** 去掉可能破坏表格布局的控制字符 */
    private static String sanitize(String s) {
        return s.replace('\n', ' ').replace('\r', ' ').replace('\t', ' ');
    }

    /** 按显示宽度右填充空格；超宽则按显示宽度截断 */
    private static String padRight(String s, int width) {
        String t = truncateToWidth(s, width);
        int w = displayWidth(t);
        return w >= width ? t : t + " ".repeat(width - w);
    }

    /** 按终端显示宽度截断字符串，不会切断代理对 */
    private static String truncateToWidth(String s, int maxWidth) {
        int width = 0;
        int i = 0;
        while (i < s.length()) {
            int cp = s.codePointAt(i);
            int cw = charWidth(cp);
            if (width + cw > maxWidth) break;
            width += cw;
            i += Character.charCount(cp);
        }
        return i == s.length() ? s : s.substring(0, i);
    }

    /** 计算字符串在等宽终端中的显示宽度 */
    public static int displayWidth(String s) {
        int width = 0;
        for (int i = 0; i < s.length(); ) {
            int cp = s.codePointAt(i);
            width += charWidth(cp);
            i += Character.charCount(cp);
        }
        return width;
    }

    /** 单个码点的显示宽度 */
    private static int charWidth(int cp) {
        // 组合字符、零宽字符
        int type = Character.getType(cp);
        if (type == Character.NON_SPACING_MARK
                || type == Character.ENCLOSING_MARK
                || type == Character.COMBINING_SPACING_MARK) {
            return 0;
        }
        if (cp == 0x200B || cp == 0x200C || cp == 0x200D || cp == 0xFEFF
                || cp == 0x2060 || (cp >= 0xFE00 && cp <= 0xFE0F)) {
            return 0;
        }
        if (cp == 0x00AD) return 0; // soft hyphen

        return isWide(cp) ? 2 : 1;
    }

    /**
     * 近似 East Asian Width = W/F 的范围。
     * 覆盖中文、日文假名/汉字、韩文、全角符号、常用 emoji 等。
     */
    private static boolean isWide(int cp) {
        return (cp >= 0x1100 && cp <= 0x115F)      // Hangul Jamo
                || (cp >= 0x2E80 && cp <= 0x303E)  // CJK Radicals / Kangxi / CJK Symbols
                || (cp >= 0x3041 && cp <= 0x33FF)  // Hiragana / Katakana / Bopomofo / CJK Compat
                || (cp >= 0x3400 && cp <= 0x4DBF)  // CJK Ext A
                || (cp >= 0x4E00 && cp <= 0x9FFF)  // CJK Unified Ideographs
                || (cp >= 0xA000 && cp <= 0xA4CF)  // Yi
                || (cp >= 0xA960 && cp <= 0xA97F)  // Hangul Jamo Ext A
                || (cp >= 0xAC00 && cp <= 0xD7A3)  // Hangul Syllables
                || (cp >= 0xF900 && cp <= 0xFAFF)  // CJK Compatibility Ideographs
                || (cp >= 0xFE10 && cp <= 0xFE19)  // Vertical Forms
                || (cp >= 0xFE30 && cp <= 0xFE6F)  // CJK Compatibility Forms
                || (cp >= 0xFF00 && cp <= 0xFF60)  // Fullwidth Forms
                || (cp >= 0xFFE0 && cp <= 0xFFE6)  // Fullwidth Signs
                || (cp >= 0x1F300 && cp <= 0x1F64F) // Emoji (misc symbols/pictographs)
                || (cp >= 0x1F900 && cp <= 0x1F9FF) // Emoji (supplemental)
                || (cp >= 0x20000 && cp <= 0x2FFFD) // CJK Ext B+
                || (cp >= 0x30000 && cp <= 0x3FFFD); // CJK Ext G+
    }

    public void showMainMenu() {
        System.out.println("""
                --- Main ---
                rv	 : search / browse contacts
                ct       : add a contact
                ud	 : modify a contact
                dl	 : delete a contact
                qt       : exit
                """);
    }

    public void showRetrieveMenu() {
        System.out.println("""
                --- Browse ---
                rt	 : back
                nm       : sort by name
                nb	 : sort by number
                <text>   : search name or number
                """);
    }

    public void showCreateMenu() {
        System.out.println("""
                --- Add (type 'rt' to cancel) ---
                name  : 
                number: 
                """);
    }

    public void showModifyMenu() {
        System.out.println("""
                --- Modify (type 'rt' to cancel) ---
                1) search  : name or number
                2) pick    : name or number from result
                3) field   : name(nm) | number(nb)
                4) value   : new value
                """);
    }

    public void showDeleteMenu() {
        System.out.println("""
                --- Delete (type 'rt' to cancel) ---
                1) search  : name or number
                2) pick    : name or number from result
                3) confirm : yes | no
                """);
    }
}
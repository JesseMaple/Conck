package conck.repository;

import java.util.ArrayList;
import java.util.List;
import java.io.FileReader;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileWriter;
import conck.model.Contact;

public class FileContactRepository implements ContactRepository{
	private String filepath;
	
	public FileContactRepository(String filepath) {
		this.filepath=filepath;
	}
	
	@Override
	public List<Contact> findAll(){
		List<Contact> contacts=new ArrayList<>();
		
		try(BufferedReader br=new BufferedReader(new FileReader(filepath))){
			String line;
			while((line=br.readLine())!=null) {
				String[] parts=line.split("\\|",2);
				if(parts.length==2) {
					contacts.add(new Contact(parts[0],parts[1]));
				}
			}
		}
		catch(IOException e) {
			System.err.println("Failed to load contacts"+e.getMessage());
		}
		
		return contacts;
	}
	
	@Override
	public void saveAll(List<Contact> contacts) {
		try(BufferedWriter bw=new BufferedWriter(new FileWriter(filepath))){
			for(Contact contact:contacts) {
				if(contact!=null) {
					bw.write(contact.name()+"|"+contact.number());
					bw.newLine();
				}
			}
		}
		catch(IOException e) {
			System.err.println("Failed to save contacts"+e.getMessage());
		}
	}
}

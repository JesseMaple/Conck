package conck.repository;

import java.util.List;
import conck.model.Contact;

public interface ContactRepository {
	List<Contact> findAll();
	void saveAll(List<Contact> contacts);
}

#include <iostream>
#include <fstream>
#include <string>
#include "studentInfo.pb.h"

using namespace std;
// Iterates though all people in the StudentInfo and prints info about them.
void ListPeople(const Student::StudentInfo& address_book) {
  for (int i = 0; i < address_book.people_size(); i++) {
    const Student::Person& person = address_book.people(i);

    cout << "Person ID: " << person.id() << endl;
    cout << "  Name: " << person.name() << endl;
    if (person.email() != "") {
      cout << "  E-mail address: " << person.email() << endl;
    }

    for (int j = 0; j < person.phones_size(); j++) {
      const Student::Person::PhoneNumber& phone_number = person.phones(j);

      switch (phone_number.type()) {
        case Student::Person::MOBILE:
          cout << "  Mobile phone #: ";
          break;
        case Student::Person::HOME:
          cout << "  Home phone #: ";
          break;
        case Student::Person::WORK:
          cout << "  Work phone #: ";
          break;
      }
      cout << phone_number.number() << endl;
    }
  }
}

// Main function:  Reads the entire address book from a file and prints all
//   the information inside.
int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << endl;
    return -1;
  }

  Student::StudentInfo address_book;

  {
    // Read the existing address book.
    fstream input(argv[1], ios::in | ios::binary);
    if (!address_book.ParseFromIstream(&input)) {
      cerr << "Failed to parse address book." << endl;
      return -1;
    }
  }

  ListPeople(address_book);

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

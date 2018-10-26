#include <iostream>
#include <fstream>
#include <string>
#include "studentInfo.pb.h"

// This function fills in a Person message based on user input.
void PromptForAddress(Student::Person* person) {
  //Getting ID
  std::cout << "Enter ID number: " << std::endl;
  int id;
  std::cin >> id;
  person->set_id(id);
  std::cin.ignore(256, '\n');

  //Getting student name
  std::cout << "\nEnter name: " << std::endl;
  getline(std::cin, *person->mutable_name());

  //Enter email (optional)
  std::string email;  
  std::cout << "\nEnter email address (blank for none): " << std::endl;
  getline(std::cin, email);
  if (!email.empty()) {
    person->set_email(email);
  }

  //Getting phone number and type
  while (true) {
    std::cout << "\nEnter a phone number (or leave blank to finish): " << std::endl;
    std::string number;
    getline(std::cin, number);
    if (number.empty()) {
      break;
    }

    Student::Person::PhoneNumber* phone_number = person->add_phones();
    phone_number->set_number(number);

    std::cout << "\nIs this a mobile, home, or work phone? " << std::endl;
    std::string type;
    getline(std::cin, type);
    if (type == "mobile") {
      phone_number->set_type(Student::Person::MOBILE);
    } else if (type == "home") {
      phone_number->set_type(Student::Person::HOME);
    } else if (type == "work") {
      phone_number->set_type(Student::Person::WORK);
    } else {
      std::cout << "Unknown phone type. Using default." << std::endl;
    }
  }
}

// Main function:  Reads the entire address book from a file,
//   adds one person based on user input, then writes it back out to the same
//   file.
int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    std::cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << std::endl;
    return -1;
  }

  Student::StudentInfo address_book;

  // Read the existing address book.
  {
    std::fstream input(argv[1], std::ios::in | std::ios::binary);
    if (!input) {
      std::cout << argv[1] << ": File not found.  Creating a new file." << std::endl;
    } else if (!address_book.ParseFromIstream(&input)) {
      std::cerr << "Failed to parse address book." << std::endl;
      return -1;
    }
  }

  // Add an address.
  PromptForAddress(address_book.add_people());

  {
    // Write the new address book back to disk.
    std::fstream output(argv[1], std::ios::out | std::ios::trunc | std::ios::binary);
    if (!address_book.SerializeToOstream(&output)) {
      std::cerr << "Failed to write address book." << std::endl;
      return -1;
    }
  }

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

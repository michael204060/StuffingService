#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

class UserNotFoundException : public std::exception {
private:
    std::string message;
public:
    explicit UserNotFoundException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class IncorrectPasswordException : public std::exception {
private:
    std::string message;
public:
    explicit IncorrectPasswordException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class PasswordNotUniqueException : public std::exception {
private:
    std::string message;
public:
    explicit PasswordNotUniqueException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class EmptyFieldException : public std::exception {
private:
    std::string message;
public:
    explicit EmptyFieldException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

#endif
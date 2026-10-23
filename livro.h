#ifndef LIVRO_H
#define LIVRO_H

#include <string>

class Livro {
 private:
  int id;
  std::string nome;
  std::string autor;
  std::string isbn;
  int quantidade;
  int emprestados;

 public:
  // Construtores
  Livro();
  Livro(const std::string& n, const std::string& a, const std::string& i, int q);

  // Getters
  int getId() const;
  std::string getNome() const;
  std::string getAutor() const;
  std::string getIsbn() const;
  int getQuantidade() const;
  int getEmprestados() const;
  int getDisponiveis() const;

  // Setters
  void setId(int i);
  void setNome(const std::string& n);
  void setAutor(const std::string& a);
  void setIsbn(const std::string& i);
  void setQuantidade(int q);
  void setEmprestados(int e);
};

#endif

#include "livro.h"

Livro::Livro() : id(0), nome(""), autor(""), isbn(""), quantidade(0), emprestados(0) {}

Livro::Livro(const std::string& n, const std::string& a, const std::string& i, int q) : id (0), nome(n), autor(a), isbn(i), quantidade(q), emprestados(0) {}

int Livro::getId() const { return id; }
std::string Livro::getNome() const { return nome; }
std::string Livro::getAutor() const { return autor; }
std::string Livro::getIsbn() const { return isbn; }
int Livro::getQuantidade() const { return quantidade; }
int Livro::getEmprestados() const { return emprestados; }
int Livro::getDisponiveis() const { return quantidade - emprestados; }

void Livro::setId(int i) { id = i; }
void Livro::setNome(const std::string& n) { nome = n; }
void Livro::setAutor(const std::string& a) { autor = a; }
void Livro::setIsbn(const std::string& i) { isbn = i; }
void Livro::setQuantidade(int q) { quantidade = q; }
void Livro::setEmprestados(int e) { emprestados = e; }

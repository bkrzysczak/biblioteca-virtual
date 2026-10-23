#ifndef BANCO_H
#define BANCO_H

#include "livro.h"
#include <sqlite3.h>
#include <vector>
#include <string>

class Banco {
private:
    sqlite3* db;
    std::string caminhoDb;

    bool executarSQL(const std::string& sql);

public:
    Banco(const std::string& caminho = "biblioteca.db");
    ~Banco();

    bool conectar();
    bool desconectar();
    bool criarTabelas();

    // Operações CRUD
    bool adicionarLivro(const std::string& nome, const std::string& autor,
                        const std::string& isbn, int quantidade);
    bool removerLivro(int id);
    bool atualizarLivro(int id, const std::string& nome, 
                        const std::string& autor, const std::string& isbn, 
                        int quantidade);
    
    // Operações de empréstimo
    bool emprestarLivro(int id);
    bool devolverLivro(int id);
    
    // Consultas
    std::vector<Livro> listarTodosLivros();
    Livro buscarLivroPorId(int id);
    std::vector<Livro> buscarLivroPorNome(const std::string& nome);
    std::vector<Livro> buscarLivroPorAutor(const std::string& autor);
};

#endif


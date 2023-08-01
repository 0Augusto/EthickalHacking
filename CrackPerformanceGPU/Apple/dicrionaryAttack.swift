/*
O código usa um array dictionary para armazenar as senhas lidas do arquivo.
O código lê as senhas do arquivo passwords.txt e armazena no array dictionary.
A variável target contém a senha alvo a ser encontrada no dicionário.
O loop DispatchQueue.concurrentPerform realiza uma busca paralela em dictionary para encontrar a senha alvo (target). É utilizada uma seção crítica (objc_sync_enter e objc_sync_exit) para garantir que as impressões sejam executadas em ordem.
As informações de número de thread e contagem total de senhas são exibidas na seção crítica.

Ordem de complexidade:
A ordem de complexidade deste código é O(n), onde n é o número total de senhas lidas do arquivo e armazenadas no dicionário (dictionary).
O loop de busca DispatchQueue.concurrentPerform paralelo itera sobre dictionary, mas a quantidade de iterações é igual ao número de senhas, portanto, a complexidade é linear em relação à quantidade de senhas (dictionarySize).
A seção crítica (impressões) não afeta a complexidade geral do código, pois ocorre em tempo constante em relação ao tamanho do dicionário.
*/

import Foundation

let MAX_PASSWORD_LENGTH = 100

func main() {
    var dictionary = [String](repeating: "", count: MAX_PASSWORD_LENGTH)
    var dictionarySize = 0

    // Read passwords from a text file
    guard let inputFile = fopen("passwords.txt", "r") else {
        print("Failed to open the input file.")
        return
    }

    var password = [CChar](repeating: 0, count: MAX_PASSWORD_LENGTH)
    while fgets(&password, MAX_PASSWORD_LENGTH, inputFile) != nil {
        let len = strlen(password)
        if len > 0 && password[Int(len) - 1] == 10 { // ASCII value for newline character '\n'
            password[Int(len) - 1] = 0 // Remove the newline character
        }
        dictionary[dictionarySize] = String(cString: password)
        dictionarySize += 1
    }
    fclose(inputFile)

    let target = "target_password"

    DispatchQueue.concurrentPerform(iterations: dictionarySize) { i in
        if target == dictionary[i] {
            // Critical section for printing
            objc_sync_enter(dictionary)
            defer { objc_sync_exit(dictionary) }
            print("Password found: \(dictionary[i])")
            print("Number of i is: \(i)")
            print("Number of thread is: \(Thread.current.number)")
            print("Total number of passwords is: \(dictionarySize)")
        }
    }
}

main()


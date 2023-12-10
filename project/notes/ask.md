# Ask

### General

- [ ] Fechar o socket TCP dos dois lados

### Server Side

- [ ] Posso só criar as pastas `HOSTED`, `BIDDED` e `AUCTIONS` só quando existir pelo menos uma
- [ ] Quando colocamos o tempo em que o leilao esteve ativo é tendo em conta quando o encerramos ou quando ele devia terminar --> depende de quando é que corremos a função para o encerrar
- [ ] Podemos devolver 'NOK' quando dá algum tipo de erro ao criar ficheiros e tal?
- [ ] Ordem pela qual temos de dar prioridade aos states.
  - Exemplo: close vs bid functions
- [ ] Algumas funções que pedem a palavra pass não indicam o status a retornar
  - 'não está no enunciado'
- [x] confirmar: quando recebemos mal o comando (exemplo: password com 45 chars) mandamos `ERR` ou `NOK` ?
  - é suposto responder com `ERR` -> "If an unexpected protocol message is received, the reply will be ERR"
  - "For replies including the status field it takes the value ERR when the syntax of the request message was incorrect or when the parameter values take invalid values."

### Client Side

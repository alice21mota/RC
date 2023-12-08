# TODO

### Server side:

- [x] Acho que a parte de TCP do select não está a 100%
  - Não sei se apenas está a faltar fechar a ligação ou se falta algo mais
  - Testar: abrir o server e fazer **dois** ou mais pedidos TCP seguidos
- [x] FIX: O server só está a imprimir o que vem do TCP depois do timeout
- [ ] whiles nos reads
- [x] enviar resposta no TCP
- [ ] Ver se é mesmo suposto ter o `exit` como comando do server
- [ ] Aproveitar "os eventos gerados pelas aplicações clientes que lhe acedem de forma concorrente, para veriﬁcar se um dado leilão se encontra expirado" (ver `2.1 a)` do guia do projeto)
- [ ] Fazer as verificações dos valores recebidos
- [ ] Checkar que não há espaços entre as mensagens trocadas com o client (se houverem, tem de dar erro)
- [ ] Mensagens de erro ao ler do UDP (ver mail `Servidor AS/UDP - esclarecimento importante` do dia 23/11)
- [x] As funções de criar os ficheiros estão bué redundantes mas não sei como lidar com os erros
- [ ] Check what sould return if is an **unlogged** user **without** bids
- [ ] _maybe_ pôr as path como `const` e `getPath... functions`

- Testar
- [ ] correr o `isCorrectPassword()` com um user que não existe

### Client side:

- [ ] Imprimir mensagens de erro antes de sair
- [ ] Função de exit
- [x] Fechar sempre o socket do UDP ou deixar aberto? --> tanto faz mas o stor prefere que esteja sempre fechado
- [ ] Checkar que não há espaços entre as mensagens trocadas com o server (se houverem, tem de dar erro)

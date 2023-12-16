# TODO

### Server side:

- [x] Acho que a parte de TCP do select não está a 100%
  - Não sei se apenas está a faltar fechar a ligação ou se falta algo mais
  - Testar: abrir o server e fazer **dois** ou mais pedidos TCP seguidos
- [x] FIX: O server só está a imprimir o que vem do TCP depois do timeout
- [ ] whiles nos reads
- [x] enviar resposta no TCP
- [ ] Ver se é mesmo suposto ter o `exit` como comando do server
- [x] Aproveitar "os eventos gerados pelas aplicações clientes que lhe acedem de forma concorrente, para veriﬁcar se um dado leilão se encontra expirado" (ver `2.1 a)` do guia do projeto)
- [ ] Fazer as verificações dos valores recebidos
- [x] Checkar que não há espaços entre as mensagens trocadas com o client (se houverem, tem de dar erro)
- [ ] Mensagens de erro ao ler do UDP (ver mail `Servidor AS/UDP - esclarecimento importante` do dia 23/11)
- [x] As funções de criar os ficheiros estão bué redundantes mas não sei como lidar com os erros
- [ ] Check what sould return if is an **unlogged** user **without** bids
- [ ] _maybe_ pôr as path como `const` e `getPath... functions`
- [x] Não permitir mais do 999 auctions
- [x] Ver a diferença entre o writeFile e o createFile
- [ ] Quando temos de retornar um `status` é suposto este ser `ERR` se os parametros forem errados
- [x] Quando estou a enviar um ficheiro tenho de garantir que acaba com `\n`
- [ ] Garantir que não **envio** mais do que 50 bids -> not sure, [estava no slack](https://rc-2324.slack.com/archives/C0614L96BUN/p1701964605129539)
- [x] Alterar a data de quando os leilões expiram -> ver mail Encerramento de leilões - servidor AS
- [ ] Aceitar files com `-` e com `_` (ver email `Projecto - caracteres especiais admitidos em nome de leilão`)
- [ ] '\n'
- [ ] Checkar se o file asset exist para ver se jáhá aqueça auction (ao invés de só checkar a pasta da auction)

- Testar
- [ ] correr o `isCorrectPassword()` com um user que não existe

### Client side:

- [x] Imprimir mensagens de erro antes de sair
- [x] Função de exit
- [x] Fechar sempre o socket do UDP ou deixar aberto? --> tanto faz mas o stor prefere que esteja sempre fechado
- [ ] Checkar que não há espaços entre as mensagens trocadas com o server (se houverem, tem de dar erro)
- [x] https://rc-2324.slack.com/archives/C0614L96BUN/p1701789339515289?thread_ts=1701783007.463039&cid=C0614L96BUN
- [x] https://rc-2324.slack.com/archives/C0614L96BUN/p1701853995009919?thread_ts=1701799419.818599&cid=C0614L96BUN

### Colocar no READ.ME

- Aceitamos mais do que um espaço entre as cenas
  - Mas verificamos a existencia de um unico \n no fim
- Admitimos `.`, `-`, `_`

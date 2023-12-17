# Programming using the Sockets interface “RC Auction”

### 102500 - Alice Mota | 102618 - Ana Almeida

### Redes de computadores

The goal of this project is to implement a simple auction platform. Users can open (host) an auction to sell some asset, and close it, as well list ongoing auctions and make bids.

## Notes

- Nas trocas de mensagens entre o servidor e o utilizador, aAceitamos mais do que um espaço a separar os compos,mas verificamos sempre a existencia de um unico \n no fim
- Admitimos `.`, `-` e `_` como caracteres especiais nos nomes dos ficheiros, conforme o comunicado pelo professor via email
- No caso de existir mais do que um erro no pedido enviado pelo cliente ao utilizador, será reportado o primeiro indicado no enunciado, isto é, os erros serão reportados pela ordem de prioridade com que eram descritos no enunciado
- Do lado do servidor, o utilizador faz exit, assumimos que o próprio computador é responsável por fechar os sockets

Server side:
- [X] Acho que a parte de TCP do select não está a 100%
  - Não sei se apenas está a faltar fechar a ligação ou se falta algo mais 
  - Testar: abrir o server e fazer **dois** ou mais pedidos TCP seguidos
- [X] FIX: O server só está a imprimir o que vem do TCP depois do timeout
- [ ] whiles nos reads
- [X] enviar resposta no TCP
- [ ] Ver se é mesmo suposto ter o `exit` como comando do server

</br>

Client side:
- [ ] Imprimir mensagens de erro antes de sair
- [ ] Função de exit
- [ ] Fechar sempre o socket do UDP ou deixar aberto?
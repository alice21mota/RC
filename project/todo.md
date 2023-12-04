# Server side:
- [X] Acho que a parte de TCP do select não está a 100%
  - Não sei se apenas está a faltar fechar a ligação ou se falta algo mais 
  - Testar: abrir o server e fazer **dois** ou mais pedidos TCP seguidos
- [X] FIX: O server só está a imprimir o que vem do TCP depois do timeout
- [ ] whiles nos reads
- [X] enviar resposta no TCP
- [ ] Ver se é mesmo suposto ter o `exit` como comando do server
- [ ] Aproveitar "os eventos gerados pelas aplicações clientes que lhe acedem de forma concorrente, para veriﬁcar se um dado leilão se encontra expirado" (ver `2.1 a)` do guia do projeto)
- [ ] Fazer as verificações dos valores recebidos
- [ ] Checkar que não há espaços entre as mensagens trocadas com o client (se houverem, tem de dar erro)
- [ ] Mensagens de erro ao ler do UDP (ver mail `Servidor AS/UDP - esclarecimento importante` do dia 23/11)
- [X] As funções de criar os ficheiros estão bué redundantes mas não sei como lidar com os erros

```
Note: No guia do projeto tem funções uteis
- dar print ao tempo
- carregar todos os lances para o leil˜ao AID por ordem decrescente do nome (valor) do lance

```

</br>

# Client side:
- [ ] Imprimir mensagens de erro antes de sair
- [ ] Função de exit
- [X] Fechar sempre o socket do UDP ou deixar aberto?  --> tanto faz mas o stor prefere que esteja sempre fechado
- [ ] Checkar que não há espaços entre as mensagens trocadas com o server (se houverem, tem de dar erro)



### Test Strings

```
Era uma vez, em uma pequena vila à beira-mar, um pescador chamado João. Todas as manhãs, ele saía com seu barco para pescar nas águas calmas do oceano. Um dia, enquanto lançava sua rede, João descobriu algo extraordinário. Uma mensagem em uma garrafa flutuava na água, contando a história de um tesouro perdido. Animado, João embarcou em uma aventura que o levaria por mares desconhecidos, enfrentando desafios e fazendo novos amigos. No final, ele descobriu que o verdadeiro tesouro estava na jornada e nas memórias que criou ao longo do caminho.
```
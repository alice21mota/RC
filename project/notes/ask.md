# Ask

### General

- [x] Fechar o socket TCP dos dois lados
  - Sim.

### Server Side

- [x] Posso só criar as pastas `HOSTED`, `BIDDED` e `AUCTIONS` só quando existir pelo menos uma
  - Answer is in [slack](https://rc-2324.slack.com/archives/C0614L96BUN/p1701728614949099?thread_ts=1701720539.828539&cid=C0614L96BUN)
- [x] Quando colocamos o tempo em que o leilao esteve ativo é tendo em conta quando o encerramos ou quando ele devia terminar --> depende de quando é que corremos a função para o encerrar
  - Resposta está no mail que o stor mandou `Encerramento de leilões - servidor AS`
- [x] Podemos devolver 'NOK' quando dá algum tipo de erro ao criar ficheiros e tal?
  - Quando não sabemos o que devolver pode ser sempre
- [x] Ordem pela qual temos de dar prioridade aos states.
  - Exemplo: close vs bid functions
  - seguir a ordem do enunciado
- [x] Algumas funções que pedem a palavra pass não indicam o status a retornar quando a password não corrsesponde à do user
  - 'não está no enunciado'
  - closeAuction
  - 'NOK'
- [x] confirmar: quando recebemos mal o comando (exemplo: password com 45 chars) mandamos `ERR` ou `NOK` ?
  - é suposto responder com `ERR` -> "If an unexpected protocol message is received, the reply will be ERR"
  - "For replies including the status field it takes the value ERR when the syntax of the request message was incorrect or when the parameter values take invalid values."
- [x] Receber `LMB UID outra cena` deve dar erro?
  - `ERR`
- [x] Números dos listen
  - Pode ser 5 (kinda indiferente)

### Client Side

- [ ] Onde guardar os files que são descarregados
- [x] Valor máximo da bid
  - 6 dígitos

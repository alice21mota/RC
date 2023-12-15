# Utils

```
Note: No guia do projeto tem funções uteis
- dar print ao tempo
- carregar todos os lances para o leil˜ao AID por ordem decrescente do nome (valor) do lance

** faltam aqui cenas **
```

### Test Strings

```
Era uma vez, em uma pequena vila à beira-mar, um pescador chamado João. Todas as manhãs, ele saía com seu barco para pescar nas águas calmas do oceano. Um dia, enquanto lançava sua rede, João descobriu algo extraordinário. Uma mensagem em uma garrafa flutuava na água, contando a história de um tesouro perdido. Animado, João embarcou em uma aventura que o levaria por mares desconhecidos, enfrentando desafios e fazendo novos amigos. No final, ele descobriu que o verdadeiro tesouro estava na jornada e nas memórias que criou ao longo do caminho.
```

### Test Commands

- Send UDP to my server:

  ```
  nc -u localhost 58000
  ```

- Send UDP to `tejo`:

  ```
  nc -u tejo.tecnico.ulisboa.pt 58011
  ```

- Send TCP to my server:

  ```
  nc -u localhost 58000
  ```

- Send TCP to `tejo`:

  ```
  nc tejo.tecnico.ulisboa.pt 58011
  ```

- Use client provided by professor:

  1. Connect to sigma

  ```
  ssh sigma.ist.utl.pt -l ist1102618
  ```

  2. Download server

  ```
  git clone https://ana-mc-almeida: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@github.com/alice21mota/RC.git
  ```

  3. Compile and run server

  ```
  make
  ./server
  ```

  4. Get IP adress

  ```
  ip addr
  ```

  5. Send request to `tejo`

  ```
  echo "ip_do_sigma 58000 n_do_script
    " | nc tejo.tecnico.ulisboa.pt 59000 > report.html
  ```

echo "193.136.128.109 58000 5
" | nc tejo.tecnico.ulisboa.pt 59000 > report_05.html

### Tests

```
LIN 102618 password
RLI REG
UNR 102618 password
RUR OK
LIN 102618 aaaaaaaa
RLI REG
```

```
mkdir USERS
mkdir USERS/111111
mkdir USERS/222222
mkdir USERS/333333
mkdir USERS/444444
echo "aaaaaaaa" > USERS/111111/111111_pass.txt
echo "bbbbbbbb" > USERS/222222/222222_pass.txt
echo "cccccccc" > USERS/333333/333333_pass.txt
echo "dddddddd" > USERS/444444/444444_pass.txt
```

```
echo "193.136.128.109 58000 21" | nc tejo.tecnico.ulisboa.pt 59000 > report_21.html
echo "193.136.128.109 58000 22" | nc tejo.tecnico.ulisboa.pt 59000 > report_22.html
echo "193.136.128.109 58000 23" | nc tejo.tecnico.ulisboa.pt 59000 > report_23.html
echo "193.136.128.109 58000 24" | nc tejo.tecnico.ulisboa.pt 59000 > report_24.html
echo "193.136.128.109 58000 25" | nc tejo.tecnico.ulisboa.pt 59000 > report_25.html
echo "193.136.128.109 58000 26" | nc tejo.tecnico.ulisboa.pt 59000 > report_26.html
echo "193.136.128.109 58000 27" | nc tejo.tecnico.ulisboa.pt 59000 > report_27.html
echo "193.136.128.109 58000 28" | nc tejo.tecnico.ulisboa.pt 59000 > report_28.html
echo "193.136.128.109 58000 29" | nc tejo.tecnico.ulisboa.pt 59000 > report_29.html
```

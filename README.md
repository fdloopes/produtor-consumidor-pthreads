Especificação:

Os produtores e os consumidores devem ser implementados com threads e compartilham um buffer (de tamanho limitado). Os produtores inserem dados no buffer e os consumidores retiram. A política de manipulação do buffer é FIFO.

Cada produtor, ao ser lançado, recebe dois parâmetros: um valor V representando um valor inteiro e um endereço de memória B, que represente o buffer. Os produtores executam um laço com V iterações, gerando valores aleatórios (rand) e inserindo-os em B. Como o tamanho do buffer é limitado, caso não tenha espaço disponível para armazenamento, o produtor permanece bloqueado. Ao final do laço, o produtor retorna.

Cada consumidor, ao ser lançado, recebe um único parâmetro: um endereço B de memória representando o buffer. Os consumidores executam um laço infinito, retirando do buffer um valor e verificando se o valor é um número primo. Caso o valor seja primo, imprimir na tela o thread_id do thread consumidor e o número primo encontrado no seguinte formato [id:v]. Importante: caso o valor retirado do buffer seja -1 (um negativo), o consumidor deve abandonar o thread. Caso o buffer esteja vazio, o consumidor deve ficar bloqueado aguardando que um valor seja inserido.

O programa principal deve lançar os produtores e os consumidores e, então, aguardar o término os produtores para então inserir no buffer tantos valores -1 quanto for o número de consumidores instanciados para poder aguardar o término dos consumidores.

Entradas:

- Valor v de iterações por Produtor

- Número p e c para o número de Produtores e Consumidores

- Tamanho t para o tamanho máximo do buffer

Infos:

- Author: Felipe Lopes

- Data_inicio: 15/09/19

- Data_termino: 19/09/19

# Kernel-Mode-Trojan
Simples trojan para literalmente, apagar o seu computador (incluindo a System32)

Como ele funciona?-
O driver é um driver de minifiltro, que registra operações no sistema de arquivo, então, sempre que uma requisição for criada, e passada para o nosso driver, e ele deleta este arquivo

E possível para-ló?-
Não! Após você executar com o comando "sc", você não poderá para-ló.

Use apenas para fins de testes, ou para zoar em uma máquina virtual ;)

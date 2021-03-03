# Kernel-Mode-Trojan
Simples trojan para literalmente, apagar o seu computador (incluindo a System32)

Como ele funciona?-

O driver é um driver de minifiltro, que registra operações no sistema de arquivo, então, sempre que uma requisição for criada, é passada para o nosso driver, e ele deleta este arquivo

E possível parar o driver?-

Não! Após você executar com o comando "sc", você não poderá para-ló.



Use apenas para fins de testes, ou para zoar em uma máquina virtual ;)

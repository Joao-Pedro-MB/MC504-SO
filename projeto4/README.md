# Project 4 - Patch the linux

## Introdução
Neste projeto tivemos um primeiro contato com o codigo do kernel linux. Para isso seguimos os dois tutariais abaixo para realizar uma pequena modificacao com printk e consertar alguns trechos de codigo com auxilio do script [checkpatch.pl](https://elixir.bootlin.com/source/scripts/checkpatch.pl).

- [Compilando o Kernel](https://docs.lkcamp.dev/unicamp_group/boot/)
- [Primeira contribuicao](https://docs.lkcamp.dev/unicamp_group/first_contrib/)

o resultado pode ser visto no [video](https://drive.google.com/drive/folders/1oD-WFWgC7LmLZpAiVPREkj56SwZHZ2sQ?usp=sharing)

O resultado das modificacoes pode ser encontrado no arquivo [patch](./patches/0001-fixing-precendence-issue-in-gcam-argument.patch)

Alem disso tambem fizemos algumas modificacoes extras que podem ser vistas no arquivo [patch](./patches/patchfile.patch)

OBS: vale ressaltar que durante a instalacao do virtme tivemos algumas dificuldades com a criacao da maquina. Alem disso tbm  tivemos dificuldades para a compilacao do kernel inicialmente. Os recursos que nos ajudaram foram a lista de dependencias:

```BASH
apt-get -y -q install          \
  bc                           \
  flex                         \
  bison                        \
  build-essential              \
  git                          \
  libncurses-dev               \
  libssl-dev                   \
  libelf-dev                   \
  u-boot-tools                 \
  wget                         \
  xz-utils                     \
  qemu-kvm                     \
  iproute2                     \
  python3                      \
  python3-pip
  ```
E a instalacao do virtme pelo seguinte passo a passo:

```BASH
git clone https://github.com/ezequielgarcia/virtme.git
cd virtme
sudo ./setup.py install
```

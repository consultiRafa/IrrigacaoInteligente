# IrrigacaoInteligente

Esse projeto contém os códigos do System Judite Smart Irrigation, protótipo de um dispositivo baseado em Arduino para o manejo inteligente 
da irrigação por aspersão na agricultura familiar. Seu objetivo é constituir uma alternativa tecnlógica, acessível e flexível para pequenos
produtores rurais, contribuindo para a automatizar a rega em sistemas de monoculturas, economizar  recursos hídricos/enérgiticos e reduzir
o impacto ambiental dessa atividade. Para isso, utiliza estimativa de Evapotranspiração da Cultura (método Hargreaves-Samani) para
determinar a demanda hídrica do campo de irrigação e assim implementar um manejo controlado da água no plantio. A Arquitetura do dispositivo 
está dividida em dois núcleos Arduino: Uno R3 e Pro Mini. O primeiro deles controla a interface do sistema que é composta pelo display
TFT e os botões. O segundo realiza os cálculos agrometereológicos e gerencia os demais componentes: sensor de temperatura do ar, 
sensor de fluxo de água, relê/válvula solonoide e relógio (RTC). São incorporados, portanto, comunicação serial entre as placas.

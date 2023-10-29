/**
  * Captura de sinais eletricos via eletrodos. Para o TCC - Unicarioca 2023.2
  * Arduino: mega 2560
  * Autor: Douglas Araujo Silva
  * Data: 29/10/2023
  * Observação: Este código foi criado e calibrado pelo Douglas para o seu TCC sobre: O USO DAS SINAPSES NEURAIS NA COMPUTAÇÃO
*/

/* DECLARAÇÃO DE VÁRIAVEIS */
	int   pinoEletrodo        = A15;	// Pino analógico onde o eletrodo está conectado (vermelho capta) (preto terra)
	int   ledSinal            = 7;		// indica a porta em que o led está conectado
	float reducaoTensao       = 0.5;	// Fator de redução de tensão em % (1 = 100%)
	float VoltagemReferencial = 5;		// valor de referencia da voltagem com que o arduino mega AT2560 trabalha (4.5v ~ 5.5v)
	int   bits		  = 9;		// váriavel que armazenará a quantidades de bits do arduino (começando do 0 até 9 = 10)
	int   valorOnda           = 1023;	// valor analógico em quantidade de BITS, com que o mega AT2560 trabalha com 10 bits (2^N - 1)
	int   valorAnalogico      = 0;		// váriavel que armazenará o valor elétrico lido
	float tensaoLida          = 0;		// váriavel que armazenará o resultado da formula de tensão
	int   casasDecimais       = 3;		// quantidade de casas decimais para a tensão lida
	unsigned long tmpLeitura  = 20;		// tempo para registro da leitura em microsegundo
	bool lerTensao            = false;	// variavel para ler a tensao (onda modular, captando sinais eletricos) ou ler em binário
	unsigned long tmpbinario  = 20;		// tempo para registro da leitura em microsegundo
	String binario            = "";		// armazena o vario do binário completo para ser convertido e exibido
	int binarioConvertido     = 0;		// valor que armazenará o binário convertido para decimal, atual.
	int faixaEstavel          = 130;	// valor para calibragem, sobre a faixa em que o binario convertido esta em estado normal, antes de alguma ação
	bool lerBinario           = true;	// variavel para saber se vai ler o binário ou o binário + o número em decimal

/* INIT */
	void setup()
	{
		Serial.begin(9600); // Inicializa a comunicação serial
		analogReference(DEFAULT); // Configura a referência analógica para 5V
		pinMode(ledSinal, OUTPUT); // configura o pin onde está conectado o led para ser um output
	}

/* LOOP */
	void loop()
	{
		// Lê o valor analógico do pino
		valorAnalogico = analogRead(pinoEletrodo);

		/* Decide o que fazer: ler a tensao (osciloscópio) OU exibir os valores em binários e/ou em decimal */
		if(lerTensao)
		{
			/**
			* cálculo de calibragem para compensar, evitar interferencias e conseguir a amostragem o mais proximo do sinal correto
			*
			* fórmula: (Sinal * V) / (2^N - 1) * RT
			*/
			tensaoLida = (valorAnalogico * VoltagemReferencial) / valorOnda * reducaoTensao;

			/* EXIBE O VALOR */
			Serial.println(tensaoLida,casasDecimais);

			/* ACENDE O LED PELA AÇÃO */
			if(tensaoLida > 0)
			{
				// acende o led
				digitalWrite(ledSinal, HIGH);
				// deixa o mesmo tempo da leitura
				delay(tmpLeitura);
				// apaga o led
				digitalWrite(ledSinal, LOW);
			}

			/* TEMPO PARA CAPTAÇÃO E LEITURA */
			delay(tmpLeitura);
		}
		else
		{
			/* EXIBIÇÃO E CONVERSAO DO SINAL ELETRICO EM BINÁRIO */
			for (int i = bits; i >= 0; i--)
			{
				// armazena o binario na string
				binario += bitRead(valorAnalogico, i);

				// adiciona os bits, convertidos do sinal analógico, um ao lado do outro, em uma cadeia de bits
				Serial.print(bitRead(valorAnalogico, i));
			}

			// converte o binario completo para um numero em decimal
			binarioConvertido = strtol(binario.c_str(), NULL, 2);

			/* EXIBIÇÃO - verifica se exibe o decimal */
			if(!lerBinario)
			{
				Serial.print("\n------- binario convertido em decimal: ");
				Serial.print(binarioConvertido); // exibe o valor convertido do binário
				Serial.print(" ---------\n");
			}
			else
			{
				// se não, apenas quebra a linha da exibição dos binários
				Serial.println();
			}

			/* ACENDE O LED PELA AÇÃO */
			if(binarioConvertido >= faixaEstavel)
			{
				// acende o led
				digitalWrite(ledSinal, HIGH);
				// deixa o mesmo tempo da leitura
				delay(tmpbinario);
				// apaga o led
				digitalWrite(ledSinal, LOW);
				// para o código para ler no terminal o sinal captado
				// exit(0);
			}

			// limpa a variavel do binario
			binario = "";

			/* TEMPO MAIOR PARA A LEITURA EM BINARIO */
			delay(tmpbinario);
		}
	}
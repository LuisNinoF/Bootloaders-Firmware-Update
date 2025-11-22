#include <sd_card_spi.h>

SPI_HandleTypeDef hspi1;


/*MCU            SD Card
 *
 * PA9        :   CS
 *
 * PA5(SCK)   :   SCK
 * PA6(MISO)  :   MISO
 * PA7(MOSI)  :   MOSI
 *
 * GND        :   GND
 * 3.3V       :   VCC*/


#define GPIOAEN		(1U<<0)

void cs_pin_set(void)
{
	GPIOA->ODR |=(1U<<9);
}

void cs_pin_reset(void)
{
	GPIOA->ODR &=~(1U<<9);

}
void sd_cs_pin_init(void)
{
	/*Enable clock acces to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA9 as output pin*/
	GPIOA->MODER |=(1U<<18);
	GPIOA->MODER &=~(1U<<9);


}

void sd_card_spi_init(void)
{
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction =  SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity =  SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase =  SPI_PHASE_1EDGE;
	hspi1.Init.NSS =  SPI_NSS_SOFT;
	hspi1.Init.BuadRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;


	/*Initialize spi*/
	spi_init(&hspi1);
}

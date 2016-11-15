void Motor_Init(void);

void Motor_SetSpeed(uint8_t speed);

void Motor_Off(void);

uint32_t Motor_GetActual(void);

void Motor_IncrementSpeed(void);

uint32_t Motor_GetDesired(void);

uint32_t Motor_GetError(void);

void Motor_ResetCount(void);

void Motor_DecrementSpeed(void);

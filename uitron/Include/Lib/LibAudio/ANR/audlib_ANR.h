#ifndef AUD_ANR_API
#define AUD_ANR_API

#define MAX_STREAM  (16)
#define MAX_CHANNEL (2)

#define NOISE_DETECTING     (0)
#define NOISE_UPDATING      (1)
#define NOISE_UPDATED       (2)

struct ANR_Config {
	// Memory associated
	int MemNeeded;      // returned by AUD_ANR_PreInit()
	void *pMemBuf;      // alloc/set outside ANR library & used in AUD_ANR_Init()

	// User Configurations
	int BlkSizeW;
	int SamplingRate;
	int Stereo;
	int NRdB;           // SNR improvement
	int HPF_CutOffFreq;
	int BiasSensitive;

	// Professional Configurations
	int Noise_Est_Hold_Time;
	int ToneMinTime;
	int Spec_Bias_Low;
	int Spec_Bias_High;
	int MaxBiasLimit;
	int MCurveN1Level;
	int MCurveN2Level;

	// The following fields are filled by AUD_ANR_Detect() or set by
	// customer in AUD_ANR_Init(). If AUD_ANR_Detect() is called "after"
	// AUD_ANR_Init(), the detected results will update user configurations.
	unsigned int MaxBias;
	unsigned int Default_Bias;
	unsigned int Default_Eng;
	unsigned short Default_Spec[513];
};

extern int  AUD_ANR_GetVersion(void);
extern int  AUD_ANR_PreInit(struct ANR_Config *);
extern int  AUD_ANR_Init(int *, struct ANR_Config *);
extern void AUD_ANR_SetSNRI(int);
extern int  AUD_ANR_GetSNRI(void);
extern void AUD_ANR_Detect_Reset(int);
extern int  AUD_ANR_Detect(int, short *, struct ANR_Config *);
extern int  AUD_ANR_Main(int, short *, short *);
extern void AUD_ANR_Destroy(int *);
#endif


#define LP2 27

typedef struct {
  double history[LP2];
  unsigned int last_index;
} Filter_lp2;

static double filter_lp2_taps[LP2] = {
  0.010129277898625973,
  0.03590985027016658,
  0.053587715249546426,
  0.035360316455937235,
  -0.014555793594787503,
  -0.039528645627812144,
  -0.002775327746718854,
  0.04908545094878858,
  0.028621108102223493,
  -0.05917355814071912,
  -0.08295310428601256,
  0.06664201390349636,
  0.3101216869935679,
  0.43059643724818325,
  0.3101216869935679,
  0.06664201390349636,
  -0.08295310428601256,
  -0.05917355814071912,
  0.028621108102223493,
  0.04908545094878858,
  -0.002775327746718854,
  -0.039528645627812144,
  -0.014555793594787503,
  0.035360316455937235,
  0.053587715249546426,
  0.03590985027016658,
  0.010129277898625973
};

void filter_lp2_init(Filter_lp2* f) {
    int i;
    for(i = 0; i < LP2; ++i)
        f->history[i] = 0;
    f->last_index = 0;
}

double filter_lp2(Filter_lp2* f, double input) {
  f->history[f->last_index++] = input;
  if(f->last_index == LP2)
    f->last_index = 0;
  double acc = 0;
  int index = f->last_index, i;
  for(i = 0; i < LP2; ++i) {
    index = index != 0 ? index - 1 : LP2 - 1;
    acc += f->history[index] * filter_lp2_taps[i];
  };
  return acc;
}

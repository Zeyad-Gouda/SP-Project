// sway_plants.frag
// شيدر خاص بتحريك النباتات والعشب (تمايل الرياح)

uniform sampler2D texture; // الصورة الأصلية
uniform float time;        // الوقت عشان الحركة تكون مستمرة

void main()
{
    // 1. نجيب مكان البكسل الحالي
    vec2 pos = gl_TexCoord[0].xy;

    // 2. نحسب قوة التمايل
    // (pos.y) بتحدد مكان النبتة.. القاعدة (y=1) ثابتة، والرأس (y=0) يتحرك.
    // (1.0 - pos.y) يعني كلماطلعنا لفوق تزيد الحركة.
    float swayAmount = 0.008; // قوة التمايل (زود الرقم عشان يتحرك أكتر)
    float speed = 3.0;       // سرعة التمايل

    // المعادلة: حركة أفقية (sin) تعتمد على الوقت والارتفاع
    float wave = sin(time * speed) * swayAmount * (1.0 - pos.y);

    // 3. نعدل مكان البكسل X
    pos.x += wave;

    // 4. نلون البكسل بالصورة الجديدة
    gl_FragColor = texture2D(texture, pos);
}
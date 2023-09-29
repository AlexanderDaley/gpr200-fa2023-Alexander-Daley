//fragmentShader.frag
#version 450
out vec4 FragColor;
in vec2 UV;
uniform float iTime;
uniform vec2 iResolution;
uniform float sSunSpeed;
uniform float sunRadius;

void main(){
    
	float sunSpeed = sin(iTime*2.0*sSunSpeed);

    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = (2.0*gl_FragCoord.xy-iResolution.xy)/iResolution.y;

    //1st sun variable
    float sun1 = length(vec2(uv-vec2(cos(iTime*sSunSpeed),sunSpeed*0.5))) - sunRadius;
    sun1 = smoothstep(0.0,0.5,sun1);
    
    //2nd sun variable
    float sun2 = length(vec2(uv-vec2(-cos(iTime*sSunSpeed),sunSpeed*0.5))) - sunRadius;
    sun2 = smoothstep(0.0,0.5,sun2);

    //star variables
    float star1 = length(vec2(uv-vec2(-0.8,-0.8))) - 0.01;
    star1 = step(0.0,star1);

    float star2 = length(vec2(uv-vec2(-0.5,-0.6))) - 0.01;
    star2 = step(0.0,star2);

    float star3 = length(vec2(uv-vec2(-0.2,-0.1))) - 0.01;
    star3 = step(0.0,star3);

    float star4 = length(vec2(uv-vec2(-0.4,0.8))) - 0.01;
    star4 = step(0.0,star4);

    float star5 = length(vec2(uv-vec2(0.7,0.3))) - 0.01;
    star5 = step(0.0,star5);

    float star6 = length(vec2(uv-vec2(0.9, 0.0))) - 0.01;
    star6 = step(0.0,star6);

    float star7 = length(vec2(uv-vec2(-1.0,0.5))) - 0.01;
    star7 = step(0.0,star7);

    float star8 = length(vec2(uv-vec2(-0.7,0.1))) - 0.01;
    star8 = step(0.0,star8);

    float star9 = length(vec2(uv-vec2(0.6, -0.6))) - 0.01;
    star9 = step(0.0,star9);

    float star10 = length(vec2(uv-vec2(1.3,0.7))) - 0.01;
    star10 = step(0.0,star10);

    //Places background stars
    vec3 color = mix(vec3(1.0,1.0,1.0), 0.1*cos(iTime+uv.xyx+vec3(0,2,4)), star1);
    color = mix(vec3(1.0,1.0,1.0), color, star3);
    color = mix(vec3(1.0,1.0,1.0), color, star6);
    color = mix(vec3(1.0,1.0,1.0), color, star7);
    color = mix(vec3(1.0,1.0,1.0), color, star10);
    //places both suns
    color = mix(vec3(1.0,1.0,0.0), color, sun1);
    color = mix(vec3(1.0,0.0,0.0), color, sun2);
    //places foreground stars
    color = mix(vec3(1.0,1.0,1.0), color, star2);
    color = mix(vec3(1.0,1.0,1.0), color, star4);
    color = mix(vec3(1.0,1.0,1.0), color, star5);
    color = mix(vec3(1.0,1.0,1.0), color, star8);
    color = mix(vec3(1.0,1.0,1.0), color, star9);
    
    // Output to screen
    FragColor = vec4(color,1.0);
}

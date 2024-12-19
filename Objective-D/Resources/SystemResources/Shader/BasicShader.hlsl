cbuffer cbFrameworkInfo : register(b0) 
{
    float gfCurrentTime;
    float gfElapsedTime;
    float2 gf2CursorPos;
};

cbuffer cbGameObjectInfo : register(b1) 
{
    matrix gmtxWorld : packoffset(c0);
    float3 gf3ObjectColor : packoffset(c4);
};

cbuffer cbCameraInfo : register(b2) 
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
    float3 gf3CameraPosition : packoffset(c8);
};

// �ؽ�ó ����
cbuffer cbFlipInfo : register(b3) 
{
    int X_Flip; // �ؽ�ó �¿���� ����
    int Y_Flip; // �ؽ�ó ���Ϲ��� ����
}

// �ؽ�ó ����
cbuffer cbAlphaInfo : register(b4) 
{
    float AlphaValue;
}

// ����
cbuffer cbLightInfo : register(b5) 
{
    float3 gLightDirection;
    float3 gLightColor;
    float3 gAmbientColor;
    float gShadowStrength;
};

cbuffer cbLightUseInfo : register(b6)
{
    int UseLight;
}

Texture2D gTexture : register(t0); // �ؽ�ó ���ø��� ���� �ؽ�ó ���ҽ�
SamplerState gSampler : register(s0); // �ؽ�ó ���÷� ����

struct VS_INPUT 
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXTURECOORD;
};

struct VS_OUTPUT 
{
    float4 positionH : SV_POSITION;
    float3 positionW : POSITION;
    float3 normal : NORMAL0;
    float3 normalW : NORMAL1;
    float2 uv : TEXTURECOORD;
};

VS_OUTPUT VSTexColor(VS_INPUT input) 
{
    VS_OUTPUT output;

    output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
    output.positionH = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
    output.normalW = mul(float4(input.normal, 0.0f), gmtxWorld).xyz;
    output.normal = input.normal;
    output.uv = input.uv;
    
    if(X_Flip == 1)
        output.uv.x = 1.0f - output.uv.x;
    
    if(Y_Flip == 1)
        output.uv.y = 1.0f - output.uv.y;

    return (output);
}

float3 ComputeLightColor(VS_OUTPUT input, float4 texColor)
{
    float3 ambient = gAmbientColor * texColor.rgb;
    float3 lightDir = normalize(-gLightDirection); // ������ ����
    float3 normal = normalize(input.normalW); // ���� ���� ����ȭ
    float NdotL = max(dot(normal, lightDir), 0.0); // ���� ���Ϳ� ���� ������ ���� (0���� ������ 0 ó��)
    float3 diffuse = gLightColor * NdotL * texColor.rgb; // ��ǻ�� ����
    diffuse *= gShadowStrength;

    float3 finalColorWithLight = ambient + diffuse;
    return finalColorWithLight;
}

float4 PSTexColor(VS_OUTPUT input) : SV_TARGET
{
    float4 texColor = gTexture.Sample(gSampler, input.uv);
    float3 meshColor = gf3ObjectColor;

    // UseLight�� 0�̸� ���� ��� �� ��, 1�̸� ���� �����
    float3 finalColor = lerp(texColor.rgb, ComputeLightColor(input, texColor), UseLight);
    
    finalColor += meshColor;
    
    if(texColor.a < 0.01)
        texColor.a = 0.0;
  
    float4 outputColor = float4(finalColor, texColor.a * AlphaValue);
    return outputColor;
}
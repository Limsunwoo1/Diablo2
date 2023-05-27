struct VS_INPUT
{
    float4 position : POSITION;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
};

float2 IsometricToScreen(float2 isometricPos)
{
    float2 screenPos;
    screenPos.x = (isometricPos.x - isometricPos.y) * 0.5;
    screenPos.y = (isometricPos.x + isometricPos.y) * 0.25;
    return screenPos;
}

float4 main(PS_INPUT input) : SV_Target
{
    float2 screenSize = float2(1600.0, 900.0);
    float2 gridSize = float2(200.0, 100.0);
    float2 pixelPos = input.position.xy / screenSize;
    float2 gridPos = floor(pixelPos * gridSize);
    float2 gridOffset = frac(pixelPos * gridSize);
    
    // 격자 라인을 그리기 위한 임계값 설정
    float lineThickness = 100.f; //0.005;
    
    // 대각선 방향의 격자 라인
    float2 diag1 = float2(1.0, 1.0) / gridSize;
    float diag1Line = dot(gridOffset, diag1) - 0.5;
    
    // 반대 대각선 방향의 격자 라인
    float2 diag2 = float2(-1.0, 1.0) / gridSize;
    float diag2Line = dot(gridOffset, diag2) - 0.5;
    
    // 격자 라인 그리기
    float gridLine = smoothstep(lineThickness, -lineThickness, abs(gridOffset.x - gridOffset.y));
    gridLine = min(gridLine, smoothstep(lineThickness, -lineThickness, abs(gridOffset.x + gridOffset.y)));
    
    // 대각선 라인 그리기
    float diagLine = smoothstep(lineThickness, -lineThickness, abs(diag1Line));
    diagLine = min(diagLine, smoothstep(lineThickness, -lineThickness, abs(diag2Line)));
    
    // 격자의 색상 결정
    float4 color = lerp(float4(1.0, 1.0, 1.0, 1.0), float4(0.0, 0.0, 0.0, 0.0), gridLine);
    color = lerp(color, float4(0.0, 0.0, 0.0, 0.0), diagLine);
    
    
    return color;
}
.section .note.GNU-stack,"",@progbits
.global shader_vs
    .global shader_vs_size
    .section .rodata
shader_vs:
    .incbin "src/VectorShaders/shader.vs"
1:
shader_vs_size:
    .int 1b - shader_vs


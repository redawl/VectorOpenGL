.section .note.GNU-stack,"",@progbits
.global shader_gs
    .global shader_gs_size
    .section .rodata
shader_gs:
    .incbin "src/VectorShaders/shader.gs"
1:
shader_gs_size:
    .int 1b - shader_gs


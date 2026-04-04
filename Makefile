GEM5_BIN = ./build/RISCV/gem5.opt
CONFIG_DIR = configs/riscv
OUT_DIR = results/gemm/1024

VARIANTS = rvvector-noflag \
           rvvector-o2 \
           serial-noflag \
           serial-o2 \
           vector-noflag \
           vector-o2

TARGETS = $(addprefix $(OUT_DIR)/, $(VARIANTS))

.PHONY: all clean

all: $(TARGETS)

$(OUT_DIR)/%: $(CONFIG_DIR)/custom-riscv-gemm-%.py
	@echo "Starting simulation in background: $*"
	mkdir -p $@
	$(GEM5_BIN) -d $@ $< > $@/terminal_out.log 2>&1 &

clean:
	@echo "Cleaning results..."
	rm -rf $(OUT_DIR)
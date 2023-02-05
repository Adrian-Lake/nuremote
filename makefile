ROOT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
export ROOT_DIR

IR_DIR := input_relay/
IR_TARGETS := rpc_client.so ir_host
export IR_TARGETS

KEY_MAPPING_DIR := key_mapping/
X11_SUPPORT_TARGETS := $(KEY_MAPPING_DIR)/xmodmapDump

SERVICE_DIR := etc/
SYSTEMD_PATH := /etc/systemd/system/
IR_HOST_BIN := $(ROOT_DIR)/$(IR_DIR)/ir_host
NUREMOTE_APP := $(ROOT_DIR)/app.py




all: $(IR_TARGETS) $(X11_SUPPORT_TARGETS) 

# proxy for input_relay targets
$(IR_TARGETS):
	cd $(IR_DIR) && $(MAKE)

clean:	
	rm -f $(SERVICE_DIR)/*.service
	rm -f $(KEY_MAPPING_DIR)/xmodmapDump 
	cd $(IR_DIR) && $(MAKE) clean

$(KEY_MAPPING_DIR)/xmodmapDump:
	xmodmap -pke > $@


# installation and removal
install: install_services

install_services: all etc/nuremote-input-relay.service etc/nuremote-server.service

	sudo chown root $(IR_HOST_BIN)
	sudo chmod 700 $(IR_HOST_BIN)

	sudo cp $(SERVICE_DIR)/nuremote-input-relay.service $(SYSTEMD_PATH)
	sudo cp $(SERVICE_DIR)/nuremote-server.service $(SYSTEMD_PATH)

	sudo systemctl daemon-reload
	sudo systemctl start nuremote-input-relay
	sudo systemctl start nuremote-server


remove: remove_services 
	
remove_services: 

	sudo systemctl stop nuremote-input-relay || true
	sudo systemctl stop nuremote-server || true

	sudo systemctl disable nuremote-input-relay || true
	sudo systemctl disable nuremote-server || true

	sudo rm -f $(SYSTEMD_PATH)/nuremote-input-relay.service
	sudo rm -f $(SYSTEMD_PATH)/nuremote-server.service

	sudo systemctl daemon-reload || true
	sudo systemctl reset-failed || true

	sudo chown $(USER) $(IR_HOST_BIN) || true
	sudo chmod 700 $(IR_HOST_BIN) || true


# .service preprocessor
%.service: %.service.in
	cp $< $@
	sed -i "s|_NUREMOTE_APP|$(NUREMOTE_APP)|g" $@
	sed -i "s|_IR_HOST_BIN|$(IR_HOST_BIN)|g" $@
	sed -i "s|_ROOT_DIR|$(ROOT_DIR)|g" $@
	sed -i "s|_USER|$(USER)|g" $@
	sed -i "s|_DISPLAY|$(DISPLAY)|g" $@

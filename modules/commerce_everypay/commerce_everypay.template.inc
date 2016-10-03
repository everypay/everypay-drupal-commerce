<script type="text/javascript" src="https://button.everypay.gr/js/button.js"></script>
<script type="text/javascript">
//<![CDATA[
    var EVERYPAY_DATA = <?php echo json_encode($params)?>;
    var $PAY_FORM = jQuery("#commerce-checkout-form-review");
    var loadButton = setInterval(function () {
        var $EV_BUTTON;
        try {
            $EV_BUTTON = EverypayButton.jsonInit(EVERYPAY_DATA, $PAY_FORM);
            clearInterval(loadButton);

            // Clear any old tokens
            document.getElementById('everypayToken').value = '';

            //Bind the form submit
            $PAY_FORM.unbind('submit').bind('submit', function(e){
                //Show the button only if everypay way is selected
                var $paymentWaySelector = $PAY_FORM.find('#edit-commerce-payment-payment-method-commerce-everypaycommerce-payment-commerce-everypay');
                if($paymentWaySelector.is(':checked') && !jQuery('#everypayToken').val().length>0){
                   $EV_BUTTON.click();
                   e.preventDefault();
                }
            })
          } catch (err) {
              //console.log(err)
          }
    }, 100);

    /**
     * Populate the token and submit
     */
    var handleEverypayToken = function(msg) {
        document.getElementById('everypayToken').value = msg.token;
        $PAY_FORM.submit();
    };

    /*
     * On close of the everypay form the submit buttons
     * must be reversed to their normal state other than loading
     */
    var handleCloseEverypay = function(msg) {
        jQuery('.checkout-continue.form-submit.checkout-processed[disabled]').remove()
        jQuery('.checkout-processing').remove();
        jQuery('.checkout-continue.form-submit.checkout-processed').show()
    };
//]]
</script>

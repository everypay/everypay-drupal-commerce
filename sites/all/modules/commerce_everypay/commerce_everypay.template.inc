<script type="text/javascript" src="https://button.everypay.gr/js/button.js"></script>
<script type="text/javascript">
//<![CDATA[
    var options = {
        'key': "<?php echo $public_key ?>",
        'amount': "<?php echo $amount ?>",
        'currency': "<?php echo $currency_code ?>",
        'sandbox': "<?php echo $sandbox ?>",
        'callback': "everypayTokenCallback",
        'label': "<?php echo $label ?>"
    };
    var loadButton = setInterval(function () {
          try {
            EverypayButton.jsonInit(options, "#commerce-checkout-form-review");
            clearInterval(loadButton);
            document.getElementsByClassName('everypay-button')[0].click();
          } catch (err) { console.log(err) }
    }, 100);
    var everypayTokenCallback = function(msg) {
        var everypayToken = document.getElementById('everypayToken');
        everypayToken.value = msg.token;
        requestID = new Date().getTime();
        jQuery.ajax({
            url: 'https://api.everypay.gr/tokens/' + msg.token,

            dataType: "jsonp",

            data: {
              key: '<?php echo $public_key ?>'
            },

            // Work with the response
            success: function( response ) {
              card = response.card;
              jQuery('#everypay-card-number span').html('(' + card.type + ') ' + 'XXXX-XXXX-XXXX-' + card.last_four);
              jQuery('#everypay-holder-name span').html(card.holder_name);
              jQuery('#everypay-expiration span').html(card.expiration_month + '/' + card.expiration_year);
            }
        });
    };
//]]
</script>

// popovers initialization - on click
$('[data-toggle="popover-click"]').popover({
    html: true,
    trigger: 'click',
    placement: 'bottom',
    container: 'body',
    content: function () { return '<img height="600" src="' + $(this).data('img') + '" />'; }
});

$('[data-toggle="popover-hover"]').popover({
    html: true,
    trigger: 'hover',
    placement: 'bottom',
    content: function () { return '<img height="600" src="' + $(this).data('img') + '" />'; }
});